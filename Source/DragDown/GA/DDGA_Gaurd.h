// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DDGA_Gaurd.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGA_Gaurd : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDDGA_Gaurd();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


protected:
	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel) override;

	UPROPERTY()
	TObjectPtr<UAnimMontage> GaurdMontage;

	UPROPERTY()
	TObjectPtr < class UAbilityTask_PlayMontageAndWait > MontageTask;

	UPROPERTY()
	TObjectPtr<ACharacter> AvatarCharacter;
};
