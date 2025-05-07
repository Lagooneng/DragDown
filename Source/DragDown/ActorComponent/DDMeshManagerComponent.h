// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/DDMeshDatas.h"
#include "DDMeshManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGDOWN_API UDDMeshManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDDMeshManagerComponent();
	
	void LoadCachedMesh();
	void SetMergedMesh(EMESHID MeshID);
	void SetHairMesh(EMESHID MeshID);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDDMeshDatas> MeshDatas;

	UPROPERTY(Config)
	EMESHID MeshIDCache;
};
