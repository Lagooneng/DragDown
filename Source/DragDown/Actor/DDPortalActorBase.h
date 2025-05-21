// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DDInteractable.h"
#include "DDPortalActorBase.generated.h"

UCLASS()
class DRAGDOWN_API ADDPortalActorBase : public AActor, public IDDInteractable
{
	GENERATED_BODY()
	
public:	
	ADDPortalActorBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> InteractionPromptComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UDDPromptWidget> PromptWidget;

public:
	virtual void Interact(AActor* InterActor) override;

	virtual bool CanInteract(AActor* InterActor) override;

	virtual void BeginInteract(AActor* InterActor) override;

	virtual void EndInteract(AActor* InterActor) override;

	virtual FText GetInteractionText(AActor* InterActor) override;

protected:
	const FText DefaultDescription = FText::FromString( TEXT("Press < F >") );
};
