// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA/DDGA_ActionBase.h"
#include "DDGA_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGA_Dodge : public UDDGA_ActionBase
{
	GENERATED_BODY()
	
public:
	UDDGA_Dodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void SetData() override;

private:
	UFUNCTION()
	void OnDodgeEventReceived(FGameplayEventData Payload);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	float NecessaryStamina;
};
