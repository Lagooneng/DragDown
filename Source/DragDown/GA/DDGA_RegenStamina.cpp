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
}

void UDDGA_RegenStamina::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.1f);
	if (WaitTask)
	{
		WaitTask->OnFinish.AddDynamic(this, &UDDGA_RegenStamina::ApplyRegenEffect);
		WaitTask->ReadyForActivation();
	}
}

void UDDGA_RegenStamina::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
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
	
	if (RegenStaminaEffect == nullptr)
	{
		UE_LOG(LogDD, Warning, TEXT("UDDGA_RegenStamina - ApplyRegenEffect - No RegenStaminaEffect"));
		return;
	}

	FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());

	FGameplayEffectSpecHandle RegenStaminaEffectHandle = MakeOutgoingGameplayEffectSpec(RegenStaminaEffect);
	ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, RegenStaminaEffectHandle);

	if (!AvatarCharacter->HasAuthority() && ASC->GetSet<UDDAttributeSet>()->GetStamina() < 99.5f)
	{
		UE_LOG(LogDD, Warning, TEXT("== APPLY ATTEMPT =="));
		UE_LOG(LogDD, Warning, TEXT("IsLocallyControlled: %s"), AvatarCharacter->IsLocallyControlled() ? TEXT("YES") : TEXT("NO"));
		UE_LOG(LogDD, Warning, TEXT("HasAuthority: %s"), AvatarCharacter->HasAuthority() ? TEXT("YES") : TEXT("NO"));
		UE_LOG(LogDD, Warning, TEXT("PredictionKey Valid: %s"), ASC->ScopedPredictionKey.IsValidKey() ? TEXT("YES") : TEXT("NO"));
		UE_LOG(LogDD, Warning, TEXT("EffectSpec Valid: %s"), RegenStaminaEffectHandle.IsValid() ? TEXT("YES") : TEXT("NO"));

		const float Base = ASC->GetNumericAttributeBase(UDDAttributeSet::GetStaminaAttribute());
		const float Current = ASC->GetNumericAttribute(UDDAttributeSet::GetStaminaAttribute());
		UE_LOG(LogDD, Log, TEXT("[NetMode : %d] Get Stamina : %f"), GetWorld()->GetNetMode(), ASC->GetSet<UDDAttributeSet>()->GetStamina());

		UE_LOG(LogDD, Warning, TEXT("Stamina - Base: %f | Current: %f"), Base, Current);
	}
	
	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.1f);
	if (WaitTask)
	{
		WaitTask->OnFinish.AddDynamic(this, &UDDGA_RegenStamina::ApplyRegenEffect);
		WaitTask->ReadyForActivation();
	}
}
