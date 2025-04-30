// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_Dodge.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/DDCharacterBase.h"
#include "Misc/DateTime.h"
#include "Attribute/DDAttributeSet.h"
#include "DataAsset/DDActionAbilityData.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tag/DDTag.h"
#include "DragDown.h"

UDDGA_Dodge::UDDGA_Dodge()
{
}

void UDDGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());

	if (AvatarCharacter)
	{
		if (AvatarCharacter->GetCharacterMovement()->IsFalling())
		{
			bool bReplicatedEndAbility = true;
			bool bWasCancelled = false;
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}
	}

	DisableInput();

	PlayAnim(AbilityData->SectionName);

	if (AvatarCharacter && ASC)
	{
		ASC->AddLooseGameplayTag(DDTAG_STATE_DODGE);
		ASC->AddLooseGameplayTag(DDTAG_STATE_USINGABILITY);

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DownStaminaEffect);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(DDTAG_DATA_STAMINAUSED, -NecessaryStamina);
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
		}
	}

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		DDTAG_EVENT_DODGEEND
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_Dodge::OnDodgeEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_Dodge::SetData()
{
	Super::SetData();

	NecessaryStamina = AbilityData->NecessaryStamina;
}

void UDDGA_Dodge::OnDodgeEventReceived(FGameplayEventData Payload)
{
	UE_LOG(LogDD, Log, TEXT("[NetMode : %d] OnDodgeEventReceived"), GetWorld()->GetNetMode());

	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(DDTAG_STATE_DODGE);
	}
}

bool UDDGA_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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
