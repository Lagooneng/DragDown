// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DDInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DRAGDOWN_API IDDInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(AActor* InterActor) = 0;

	virtual bool CanInteract(AActor* InterActor) = 0;

	virtual void BeginInteract(AActor* InterActor) = 0;

	virtual void EndInteract(AActor* InterActor) = 0;

	virtual FText GetInteractionText(AActor* InterActor) = 0;
};
