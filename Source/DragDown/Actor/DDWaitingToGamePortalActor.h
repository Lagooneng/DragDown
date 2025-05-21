// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DDPortalActorBase.h"
#include "DDWaitingToGamePortalActor.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDWaitingToGamePortalActor : public ADDPortalActorBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void Interact(AActor* InterActor) override;

	virtual bool CanInteract(AActor* InterActor) override;

	virtual void BeginInteract(AActor* InterActor) override;

	virtual FText GetInteractionText(AActor* InterActor) override;

	void UpdateDescription();

	UFUNCTION()
	void UpdateAndSetDescription();

protected:
	FText CurrentDescription = FText::FromString(TEXT("Not Initialized"));
	const FText StartDescription = FText::FromString(TEXT("Press F To Start"));
	const FText NotHostDescription = FText::FromString(TEXT("Only Host Can Start"));
	const FText NotReadyDescription = FText::FromString(TEXT("Not All Players Are Ready"));
};
