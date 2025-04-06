// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA/DDGA_ActionBase.h"
#include "DDGA_PushingCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGA_PushingCharacter : public UDDGA_ActionBase
{
	GENERATED_BODY()
	
public:
	UDDGA_PushingCharacter();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnPushingEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	void ProcessPush(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY()
	TObjectPtr<class UDDAttackStateComponent> AttackStateComponent;

};
