// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_RegenStamina.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "DragDown.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Attribute/DDAttributeSet.h"


UDDGA_RegenStamina::UDDGA_RegenStamina()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FClassFinder<UGameplayEffect> RegenStaminaEffectRef(TEXT("/Game/Blueprint/GA/GE/BPGE_RegenStamina.BPGE_RegenStamina_C"));
	if (RegenStaminaEffectRef.Succeeded())
	{
		RegenStaminaEffect = RegenStaminaEffectRef.Class;
	}

	RegenTime = 0.1f;
}

void UDDGA_RegenStamina::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if ( CurrentActorInfo->AvatarActor->HasAuthority() )
	{
		WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, RegenTime);
		if (WaitTask)
		{
			WaitTask->OnFinish.AddDynamic(this, &UDDGA_RegenStamina::ApplyRegenEffect);
			WaitTask->ReadyForActivation();
		}
	}
	
}

void UDDGA_RegenStamina::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if ( WaitTask )
	{
		WaitTask->EndTask();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDDGA_RegenStamina::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancel);
}

void UDDGA_RegenStamina::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	ASC = ActorInfo->AbilitySystemComponent.Get();
}

void UDDGA_RegenStamina::ApplyRegenEffect()
{
	if (ASC == nullptr)
	{
		UE_LOG(LogDD, Warning, TEXT("UDDGA_RegenStamina - ApplyRegenEffect - No ASC"));
		return;
	}

	if ( ASC->GetSet<UDDAttributeSet>() && ASC->GetSet<UDDAttributeSet>()->GetStamina() >= 100.0f)
	{
		WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, RegenTime);
		if (WaitTask)
		{
			WaitTask->OnFinish.AddDynamic(this, &UDDGA_RegenStamina::ApplyRegenEffect);
			WaitTask->ReadyForActivation();
		}
		return;
	}
	
	if (RegenStaminaEffect == nullptr)
	{
		UE_LOG(LogDD, Warning, TEXT("UDDGA_RegenStamina - ApplyRegenEffect - No RegenStaminaEffect"));
		return;
	}

	if ( !AvatarCharacter->HasAuthority() )
	{
		UE_LOG(LogDD, Log, TEXT("ApplyRegenEffect"));
	}

	FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(RegenStaminaEffect);
	if (EffectSpecHandle.IsValid())
	{
		//EffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.StaminaUsed")), 10.0f);
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}

	WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, RegenTime);
	if (WaitTask)
	{
		WaitTask->OnFinish.AddDynamic(this, &UDDGA_RegenStamina::ApplyRegenEffect);
		WaitTask->ReadyForActivation();
	}
}
