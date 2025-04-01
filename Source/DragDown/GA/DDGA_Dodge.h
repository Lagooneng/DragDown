// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DDGA_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGA_Dodge : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDDGA_Dodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnDodgeEventReceived(FGameplayEventData Payload);

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel) override;

	UPROPERTY()
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY()
	TObjectPtr < class UAbilityTask_WaitGameplayEvent > EventTask;

	UPROPERTY()
	TObjectPtr<ACharacter> AvatarCharacter;

	bool bIsTraced;
};
