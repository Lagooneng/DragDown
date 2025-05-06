// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDMeshDatas.generated.h"

UENUM()
enum class EMESHID : uint32
{
	MERCF_01 = 101,
	MERCF_02 = 102,
	MERCF_03 = 103,
	MERCF_04 = 104,
	MERCM_01 = 201,
	MERCM_02 = 202,
	MERCM_03 = 203,
	MERCM_04 = 204,
	DRESS_01 = 301,
	DRESS_02 = 302,
	DRESS_03 = 303,
	DRESS_04 = 304,
	DEMONF_01 = 401,
	DEMONF_02 = 402,
	DEMONF_03 = 403,
	DEMONF_04 = 404,
	DEMONM_01 = 501,
	DEMONM_02 = 502,
	DEMONM_03 = 503,
	DEMONM_04 = 504,
	ASSASSIN_01 = 601,
	ASSASSIN_02 = 602,
	ASSASSIN_03 = 603,
	ASSASSIN_04 = 604,
	SKELETON_01 = 701,
	SKELETON_02 = 702,
	SKELETON_03 = 703,
	SKELETON_04 = 704,
	ARDEN_01 = 801
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
