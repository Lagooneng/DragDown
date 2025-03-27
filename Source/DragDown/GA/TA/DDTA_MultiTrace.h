// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "DDTA_MultiTrace.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDTA_MultiTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ADDTA_MultiTrace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug;
};
