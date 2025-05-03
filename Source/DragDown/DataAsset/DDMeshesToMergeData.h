// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDMeshesToMergeData.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDMeshesToMergeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray< TObjectPtr<USkeletalMesh> > MeshesToMerge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMesh> HairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeleton> Skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPhysicsAsset> PhysicsAsset; 
};
