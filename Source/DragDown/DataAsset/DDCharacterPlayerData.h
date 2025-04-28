// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDCharacterPlayerData.generated.h"

USTRUCT(BlueprintType)
struct FDDCharacterPlayerDataEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpZVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinAnalogWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingDecelerationWalking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TSubclassOf<class UAnimInstance> AnimInstance;
};

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDCharacterPlayerData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDDCharacterPlayerData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDDCharacterPlayerDataEntry CharacterPlayerData;
};
