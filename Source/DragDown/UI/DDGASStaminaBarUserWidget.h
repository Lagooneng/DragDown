// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DDGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "DDGASStaminaBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGASStaminaBarUserWidget : public UDDGASUserWidget
{
	GENERATED_BODY()
	
public:
	UDDGASStaminaBarUserWidget();

	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	void UpdateStaminaBar();
protected:
	virtual void OnStaminaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxStaminaChanged(const FOnAttributeChangeData& ChangeData);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbStaminaBar;

	float CurrentStamina = 0.0f;

	float CurrentMaxStamina = 0.1f;

	void PredictStamina();

	FTimerHandle PredictionTimer;
	float StaminaRegenTime;
	float StaminaRegenValue;
};
