// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_PushingCharacter.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GA/AT/DDAT_MultiTrace.h"
#include "GA/TA/DDTA_MultiTrace.h"
#include "Misc/DateTime.h"
#include "DragDown.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "ActorComponent/DDAttackStateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GA/DDGA_JumpPushingCharacter.h"
#include "Character/DDCharacterBase.h"
#include "Attribute/DDAttributeSet.h"

UDDGA_PushingCharacter::UDDGA_PushingCharacter()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PushingMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Montage/AM_Manny_Pushing.AM_Manny_Pushing'"));
	if ( PushingMontageRef.Succeeded() )
	{
		ActionMontage = PushingMontageRef.Object;
	}
}

void UDDGA_PushingCharacter::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bIsEventTriggered = false;

	if ( AvatarCharacter )
	{
		if (AvatarCharacter->GetCharacterMovement()->IsFalling())
		{
			if (ASC)
			{
				ASC->TryActivateAbilityByClass(UDDGA_JumpPushingCharacter::StaticClass());
			}
			
			bool bReplicatedEndAbility = true;
			bool bWasCancelled = false;
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}

	if (AttackStateComponent == nullptr)
	{
		bool bReplicatedEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (ASC)
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UsingAbility")));

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DownStaminaEffect);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("Data.StaminaUsed")), -AttackStateComponent->GetNecessaryStamina());
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
		}
	}

	FString SectionString = AttackStateComponent->GetSectionPrefix() + FString::FromInt(AttackStateComponent->GetAttackState());
	FName SectionName(*SectionString);

	// UAbilityTask_PlayMontageAndWait를 썼었는데 플레이어가 Multicast 날리는 거로 변경
	// 이유: 클라이언트A가 클라이언트 B, C의 애니메이션을 UAbilityTask_PlayMontageAndWait로 복제받으면 느림
	// RPC가 훨씬 빨라서 다른 클라이언트 애니메이션 동기화가 더 잘되고
	// 본인은 Local Prediction을 통해 지연 완화
	if (AvatarCharacter)
	{
		if (ASC)
		{
			FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
			AvatarCharacter->NetMulticastPlayAnimMontage(ActionMontage, SectionName);
		}
	}

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.PushTrigger"))
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_PushingCharacter::OnPushingEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_PushingCharacter::OnPushingEventReceived(FGameplayEventData Payload)
{
	UDDAT_MultiTrace* TraceTask = UDDAT_MultiTrace::CreateTask(this, ADDTA_MultiTrace::StaticClass());
	TraceTask->OnComplete.AddDynamic(this, &UDDGA_PushingCharacter::OnTraceResultCallback);
	TraceTask->ReadyForActivation();
}

void UDDGA_PushingCharacter::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (bIsEventTriggered) return;
	bIsEventTriggered = true;
	UE_LOG(LogDD, Log, TEXT("[NetMode : %d] OnTraceResultCallback"), GetWorld()->GetNetMode());

	if (ASC)
	{
		FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
		ProcessPush(TargetDataHandle);
		AttackStateComponent->PlusAttackState();
	}

	bool bReplicatedEndAbility = true; 
	bool bWasCancelled = false; 
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled); 
}

void UDDGA_PushingCharacter::ProcessPush(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	int32 Idx = 0;
	while (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, Idx))
	{
		ACharacter* Character = nullptr;

		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, Idx);
		if (HitResult.GetActor())
		{
			Character = Cast<ACharacter>(HitResult.GetActor());
		}
		
		if (Character)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);

			if (TargetASC && TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Dodge"))))
			{
				UE_LOG(LogDD, Log, TEXT("Character %s is dodging - skipping LaunchCharacter"), *Character->GetName());
				++Idx;
				continue;
			}

			if ( AttackStateComponent == nullptr )
			{
				UE_LOG(LogDD, Log, TEXT("UDDGA_PushingCharacter::ProcessPush - No AttackStateComponent"));
				return;
			}

			if ( AvatarCharacter == nullptr )
			{
				UE_LOG(LogDD, Log, TEXT("UDDGA_PushingCharacter::ProcessPush - No AvatarCharacter"));
				return;
			}

			FVector LaunchDirection = AvatarCharacter->GetController()->GetControlRotation().Vector(); // 내가 바라보는 방향

			FVector LaunchVelocity = LaunchDirection * AttackStateComponent->GetPower();
			LaunchVelocity.Z = 0.0f;
			LaunchVelocity.Z += AttackStateComponent->GetZPower();

			Character->LaunchCharacter(LaunchVelocity, true, true);
		}

		UE_LOG(LogDD, Log, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());

		++Idx;
	}
}

bool UDDGA_PushingCharacter::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (ASC == nullptr) return false;

	const UDDAttributeSet* AttributeSet = ASC->GetSet<UDDAttributeSet>();
	if (AttributeSet == nullptr) return false;

	float CurrentStamina = AttributeSet->GetStamina();

	if (CurrentStamina < AttackStateComponent->GetNecessaryStamina())
	{
		return false;
	}

	return true;
}

void UDDGA_PushingCharacter::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (AvatarCharacter)
	{
		AttackStateComponent = AvatarCharacter->GetComponentByClass<UDDAttackStateComponent>();
	}
}

