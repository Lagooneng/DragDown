// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDGimmickActor.generated.h"

UCLASS()
class DRAGDOWN_API ADDGimmickActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADDGimmickActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

protected:
	virtual void BeginPlay() override;

	void ActGimmick();

	UFUNCTION(BlueprintCallable)
	void OnActionFinished();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	TArray< FVector > TargetPositions;

	int32 NowPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	TArray< FRotator > TargetRotations;

	int32 NowRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick")
	int32 ActionTime;

};
