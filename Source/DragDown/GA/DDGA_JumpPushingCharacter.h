// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DDGA_JumpPushingCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGA_JumpPushingCharacter : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDDGA_JumpPushingCharacter();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnPushingEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	void ProcessPush(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel) override;

	UPROPERTY()
	TObjectPtr<UAnimMontage> PushingMontage;

	UPROPERTY()
	TObjectPtr < class UAbilityTask_PlayMontageAndWait > MontageTask;

	UPROPERTY()
	TObjectPtr < class UAbilityTask_WaitGameplayEvent > EventTask;


	UPROPERTY()
	TObjectPtr<ACharacter> AvatarCharacter;

	float Power;
	float ZPower;

	bool bIsTraced;
};
