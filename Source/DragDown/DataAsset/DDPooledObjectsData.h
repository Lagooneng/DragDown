// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDPooledObjectsData.generated.h"

USTRUCT(BlueprintType)
struct FPooledObjectEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf< AActor > PooledObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PoolingSize;
};

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDPooledObjectsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPooledObjectEntry> PooledObjects;
};
