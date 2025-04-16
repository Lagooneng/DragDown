// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DDPoolable.h"
#include "DDBulletBase.generated.h"

UCLASS()
class DRAGDOWN_API ADDBulletBase : public AActor, public IDDPoolable
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

// Pooling
protected:
	void OnRetrievedFromPool() override;
	void OnReturnedToPool() override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastOnRetrievedFromPool(FVector NewLocation, FRotator NewRotation);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastOnReturnedToPool();

	void PoolBullet();

	UPROPERTY()
	FTimerHandle PoolingTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletLivingTime;

protected:
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
};
