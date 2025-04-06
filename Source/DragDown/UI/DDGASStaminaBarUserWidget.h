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

	void UpdateStaminaBar(float StaminaToDisplay);
protected:
	virtual void OnStaminaChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxStaminaChanged(const FOnAttributeChangeData& ChangeData);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbStaminaBar;

	UPROPERTY(EditAnywhere, Category = "UI|Stamina")
	float InterpSpeed = 5.0f;

	float InterpolatedStamina = 0.0f;

	float CurrentStamina = 0.0f;

	float CurrentMaxStamina = 0.1f;

// Preidction
private:
	void PredictStaminaUI();

	float PredictionDeltaValue;
	float PredictionPeriod;

	FTimerHandle StaminaPredictionHandle;
};
