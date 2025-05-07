// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDUICamera.generated.h"

UCLASS()
class DRAGDOWN_API ADDUICamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ADDUICamera();

public:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneCaptureComponent2D> Capture;

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
	FVector Offset;
};
