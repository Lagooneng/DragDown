// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDMeshDatas.generated.h"

UENUM()
enum class EMESHID : uint32
{
	MERC = 1,
	DRESS = 2
};

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDMeshDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EMESHID, TObjectPtr<class UDDMeshesToMergeData>> Meshes;
};
