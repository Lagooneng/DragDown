// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDPeriodicLauncher.generated.h"

UCLASS()
class DRAGDOWN_API ADDPeriodicLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	ADDPeriodicLauncher();
	void Spawn();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Launcher")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launcher")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY()
	FTimerHandle SpawningHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launcher")
	FRotator Rot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launcher")
	float Offset;
};
