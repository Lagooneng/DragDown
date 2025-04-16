// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDPeriodicSpawner.generated.h"

UCLASS()
class DRAGDOWN_API ADDPeriodicSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ADDPeriodicSpawner();
	void Spawn();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorSpawned")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY()
	FTimerHandle SpawningHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorSpawned")
	FRotator SpawningRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorSpawned")
	float Offset;
};
