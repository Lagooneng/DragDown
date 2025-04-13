// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDBulletBase.generated.h"

UCLASS()
class DRAGDOWN_API ADDBulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ADDBulletBase();

protected:
	UFUNCTION()
	void OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	void PushCharacter(ACharacter* Character);

	UPROPERTY()
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY()
	TObjectPtr<class UProjectileMovementComponent> Movement;

	UPROPERTY(EditAnywhere)
	float Power;
};
