// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "DDGroundFrictionData.generated.h"

USTRUCT(BlueprintType)
struct FFrictionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroundFriction = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFrictionFactor = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingDecelerationWalking = 2000.0f;
};

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGroundFrictionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrictionData")
	TMap<TEnumAsByte<EPhysicalSurface>, FFrictionEntry > FrictionData;
};
