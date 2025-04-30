// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_PushingCharacter.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA/AT/DDAT_MultiTrace.h"
#include "GA/TA/DDTA_MultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActorComponent/DDAttackStateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GA/DDGA_JumpPushingCharacter.h"
#include "Character/DDCharacterBase.h"
#include "Attribute/DDAttributeSet.h"
#include "DataAsset/DDActionAbilityData.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tag/DDTag.h"
#include "DragDown.h"

UDDGA_PushingCharacter::UDDGA_PushingCharacter()
{
}

void UDDGA_PushingCharacter::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());

	bIsEventTriggered = false;

	if ( AvatarCharacter )
	{
		if (AvatarCharacter->GetCharacterMovement()->IsFalling())
		{
			if (ASC)
			{
				ASC->TryActivateAbilityByClass(JumpPushing); 
			}
			
			bool bReplicatedEndAbility = true;
			bool bWasCancelled = false;
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}
	}

	DisableInput();

	if (AttackStateComponent == nullptr)
	{
		bool bReplicatedEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (ASC)
	{
		ASC->AddLooseGameplayTag(DDTAG_STATE_USINGABILITY);

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DownStaminaEffect);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude( DDTAG_DATA_STAMINAUSED, -AttackStateComponent->GetNecessaryStamina());
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
		}
	}

	FString SectionString = AttackStateComponent->GetSectionPrefix() + FString::FromInt(AttackStateComponent->GetAttackState());
	FName SectionName(*SectionString);

	PlayAnim(SectionName);

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		DDTAG_EVENT_PUSHTRIGGER
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_PushingCharacter::OnPushingEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_PushingCharacter::SetData()
{
	Super::SetData();

	JumpPushing = AbilityData->NextAbility;
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

			if (TargetASC && TargetASC->HasMatchingGameplayTag(DDTAG_STATE_DODGE))
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

