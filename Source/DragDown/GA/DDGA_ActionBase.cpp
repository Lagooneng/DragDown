// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_ActionBase.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/DDCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DragDown.h"
#include "Tag/DDTag.h"

UDDGA_ActionBase::UDDGA_ActionBase()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DownStaminaEffectRef(TEXT("/Game/Blueprint/GA/GE/BPGE_DownStamina.BPGE_DownStamina_C"));
	if (DownStaminaEffectRef.Succeeded())
	{
		DownStaminaEffect = DownStaminaEffectRef.Class;
	}

	bIsEventTriggered = false;
	ActivationBlockedTags.AddTag(DDTAG_STATE_USINGABILITY);
}

void UDDGA_ActionBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActivationInfo.GetActivationPredictionKey().IsValidKey())
	{
		FDateTime Now = FDateTime::Now();
		FString Timestamp = FString::Printf(TEXT("%d-%02d-%02d %02d:%02d:%02d.%03d"),
			Now.GetYear(), Now.GetMonth(), Now.GetDay(),
			Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());

		UE_LOG(LogDD, Warning, TEXT("[%s][NetMode %d] Client-side(%s) prediction running"),
			*Timestamp, GetWorld()->GetNetMode(), *ActorInfo->AvatarActor.Get()->GetName());
	}

	if (HasAuthority(&ActivationInfo))
	{
		FDateTime Now = FDateTime::Now();
		FString Timestamp = FString::Printf(TEXT("%d-%02d-%02d %02d:%02d:%02d.%03d"),
			Now.GetYear(), Now.GetMonth(), Now.GetDay(),
			Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());

		UE_LOG(LogDD, Warning, TEXT("[%s][NetMode %d] Server-side(%s) authority running"),
			*Timestamp, GetWorld()->GetNetMode(), *ActorInfo->AvatarActor.Get()->GetName());
	}

	bIsEventTriggered = false;
}

void UDDGA_ActionBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	bIsEventTriggered = false;

	if (EventTask && EventTask->IsActive())
	{
		EventTask->EndTask();
	}

	// for Local Prediction Role Back
	if (AvatarCharacter)
	{
		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(DDTAG_STATE_USINGABILITY);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDDGA_ActionBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	AvatarCharacter = Cast<ADDCharacterBase>(ActorInfo->AvatarActor.Get());
	ASC = ActorInfo->AbilitySystemComponent.Get();
}
