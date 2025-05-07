// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDMeshDatas.generated.h"

UENUM(BlueprintType)
enum class EMESHID : uint8
{
	NONE = 0		UMETA(DisplayName = "None"),
	MERCF_01		UMETA(DisplayName = "Mercenary Female 01"),
	MERCF_02		UMETA(DisplayName = "Mercenary Female 02"),
	MERCF_03		UMETA(DisplayName = "Mercenary Female 03"),
	MERCF_04		UMETA(DisplayName = "Mercenary Female 04"),
	MERCM_01		UMETA(DisplayName = "Mercenary Male 01"),
	MERCM_02		UMETA(DisplayName = "Mercenary Male 02"),
	MERCM_03		UMETA(DisplayName = "Mercenary Male 03"),
	MERCM_04		UMETA(DisplayName = "Mercenary Male 04"),
	DRESS_01		UMETA(DisplayName = "Dress 01"),
	DRESS_02		UMETA(DisplayName = "Dress 02"),
	DRESS_03		UMETA(DisplayName = "Dress 03"),
	DRESS_04		UMETA(DisplayName = "Dress 04"),
	DEMONF_01		UMETA(DisplayName = "Demon Female 01"),
	DEMONF_02		UMETA(DisplayName = "Demon Female 02"),
	DEMONF_03		UMETA(DisplayName = "Demon Female 03"),
	DEMONF_04		UMETA(DisplayName = "Demon Female 04"),
	DEMONM_01		UMETA(DisplayName = "Demon Male 01"),
	DEMONM_02		UMETA(DisplayName = "Demon Male 02"),
	DEMONM_03		UMETA(DisplayName = "Demon Male 03"),
	DEMONM_04		UMETA(DisplayName = "Demon Male 04"),
	ASSASSIN_01		UMETA(DisplayName = "Assassin 01"),
	ASSASSIN_02		UMETA(DisplayName = "Assassin 02"),
	ASSASSIN_03		UMETA(DisplayName = "Assassin 03"),
	ASSASSIN_04		UMETA(DisplayName = "Assassin 04"),
	ARDEN_01		UMETA(DisplayName = "Arden 01")
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
