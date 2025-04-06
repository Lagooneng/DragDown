// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_JumpPushingCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA/AT/DDAT_MultiTrace.h"
#include "GA/TA/DDTA_MultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/DDCharacterBase.h"
#include "Attribute/DDAttributeSet.h"
#include "Tag/DDTag.h"
#include "DragDown.h"


UDDGA_JumpPushingCharacter::UDDGA_JumpPushingCharacter()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PushingMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Montage/AM_Manny_JumpPushingCharacter.AM_Manny_JumpPushingCharacter'"));
	if (PushingMontageRef.Succeeded())
	{
		ActionMontage = PushingMontageRef.Object;
	}

	bIsEventTriggered = false;
	Power = 400.0f;
	ZPower = 800.0f;

	NecessaryStamina = 40.0f;
}

void UDDGA_JumpPushingCharacter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bIsEventTriggered = false;

	if (ASC)
	{
		ASC->AddLooseGameplayTag(DDTAG_STATE_USINGABILITY);

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DownStaminaEffect);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(DDTAG_DATA_STAMINAUSED, -NecessaryStamina);
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
		}
	}

	// UAbilityTask_PlayMontageAndWait를 썼었는데 플레이어가 Multicast 날리는 거로 변경
	// 이유: 클라이언트A가 클라이언트 B, C의 애니메이션을 UAbilityTask_PlayMontageAndWait로 복제받으면 느림
	// RPC가 훨씬 빨라서 다른 클라이언트 애니메이션 동기화가 더 잘되고
	// 본인은 Local Prediction을 통해 지연 완화
	if (AvatarCharacter)
	{
		if (ASC)
		{
			FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
			AvatarCharacter->NetMulticastPlayAnimMontage(ActionMontage, FName("Start"));
		}
	}

	if ( AvatarCharacter ) 
	{
		AvatarCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 300.0f), true, true);
	}

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		DDTAG_EVENT_PUSHTRIGGER
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_JumpPushingCharacter::OnPushingEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_JumpPushingCharacter::OnPushingEventReceived(FGameplayEventData Payload)
{
	UDDAT_MultiTrace* TraceTask = UDDAT_MultiTrace::CreateTask(this, ADDTA_MultiTrace::StaticClass());
	TraceTask->OnComplete.AddDynamic(this, &UDDGA_JumpPushingCharacter::OnTraceResultCallback);
	TraceTask->ReadyForActivation();
}

void UDDGA_JumpPushingCharacter::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (bIsEventTriggered) return;
	bIsEventTriggered = true;
	UE_LOG(LogDD, Log, TEXT("OnTraceResultCallback"));

	if ( AvatarCharacter )
	{
		AvatarCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 300.0f), true, true);
	}

	if (ASC)
	{
		FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
		ProcessPush(TargetDataHandle);
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UDDGA_JumpPushingCharacter::ProcessPush(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
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

			if (TargetASC && TargetASC->HasMatchingGameplayTag(DDTAG_STATE_DODGE) )
			{
				UE_LOG(LogDD, Log, TEXT("Character %s is dodging - skipping LaunchCharacter"), *Character->GetName());
				++Idx;
				continue;
			}

			if (AvatarCharacter == nullptr)
			{
				UE_LOG(LogDD, Log, TEXT("UDDGA_PushingCharacter::ProcessPush - No AvatarCharacter"));
				return;
			}

			FVector LaunchDirection = AvatarCharacter->GetController()->GetControlRotation().Vector(); // 내가 바라보는 방향
			FVector LaunchVelocity = LaunchDirection * Power;
			LaunchVelocity.Z = 0.0f;
			LaunchVelocity.Z += ZPower;

			Character->LaunchCharacter(LaunchVelocity, true, true);
		}

		UE_LOG(LogDD, Log, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());

		++Idx;
	}
}

bool UDDGA_JumpPushingCharacter::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (ASC == nullptr) return false;

	const UDDAttributeSet* AttributeSet = ASC->GetSet<UDDAttributeSet>();
	if (AttributeSet == nullptr) return false;

	float CurrentStamina = AttributeSet->GetStamina();

	if (CurrentStamina < NecessaryStamina)
	{
		return false;
	}

	return true; 
}
