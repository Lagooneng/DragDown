// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "DDGASUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGASUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual void SetOwner(AActor* InOwner);
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<AActor> Owner;
};
