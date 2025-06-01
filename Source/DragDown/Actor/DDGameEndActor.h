// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DDInteractable.h"
#include "DDGameEndActor.generated.h"

UCLASS()
class DRAGDOWN_API ADDGameEndActor : public AActor, public IDDInteractable
{
	GENERATED_BODY()
	
public:	
	ADDGameEndActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Interact(AActor* InterActor) override;
	virtual bool CanInteract(AActor* InterActor) override;
	virtual void BeginInteract(AActor* InterActor) override;
	virtual void EndInteract(AActor* InterActor) override;
	virtual FText GetInteractionText(AActor* InterActor) override;

protected:
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void EndGame(AActor* InterActor);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastInteracted();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> InteractionPromptComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UDDPromptWidget> PromptWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeDuration;

	const FText InteractionText = FText::FromString(TEXT("Press 'F' to Win Game!"));

	FTimerHandle TransitionTimer;

	bool bIsInteracted;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Launch;

};
