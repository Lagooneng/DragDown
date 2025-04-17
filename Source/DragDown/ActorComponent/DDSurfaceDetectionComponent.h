// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDSurfaceDetectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGDOWN_API UDDSurfaceDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDDSurfaceDetectionComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMovementUpdated(float DeltaTime, FVector OldLocation, FVector OldVelocity);

	void SetMovementForFriction(EPhysicalSurface NewSurface);
	
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComp;

	UPROPERTY()
	TObjectPtr<class UDDGroundFrictionData> GroundFrictionData;
};
