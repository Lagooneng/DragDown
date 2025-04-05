// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DDGA_RegenStamina.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGA_RegenStamina : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDDGA_RegenStamina();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel) override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION()
	void ApplyRegenEffect();

	UPROPERTY()
	TSubclassOf< class UGameplayEffect > RegenStaminaEffect;

	UPROPERTY()
	TObjectPtr<class ACharacter> AvatarCharacter; 

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UAbilityTask_WaitDelay> WaitTask;

	float RegenTime;
	float RegenValue;
};
