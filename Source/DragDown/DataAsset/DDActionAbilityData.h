// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDActionAbilityData.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDActionAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common Property")
	TObjectPtr<UAnimMontage> ActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common Property")
	TSubclassOf< class UGameplayEffect > DownStaminaEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optional Property")
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optional Property")
	float ZPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optional Property")
	float NecessaryStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Optional Property")
	TSubclassOf< class UGameplayAbility > NextAbility;
};
