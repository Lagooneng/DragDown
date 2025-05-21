// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDInteractionManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGDOWN_API UDDInteractionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDDInteractionManagerComponent();

	virtual void BeginPlay() override;

// Input
public:
	void SetInteractionInputComponent();

protected:
	void InteractionInputPressed();

	UFUNCTION(Server, Reliable)
	void ServerInteractionInputPressed();

	void HandleInteractionInputPressed();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

protected:
	UFUNCTION()
	void OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TObjectPtr<AActor> CurrentInteractionActor;
};
