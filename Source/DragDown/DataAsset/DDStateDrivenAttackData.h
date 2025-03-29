// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDStateDrivenAttackData.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDStateDrivenAttackData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "Name")
	uint8 MaxStateCount;

	UPROPERTY(EditAnywhere, Category = "ComboData")
	TArray< float > AttackPower;

	UPROPERTY(EditAnywhere, Category = "ComboData")
	TArray< float > ZPower;
};
