// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDGASStaminaBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/DDAttributeSet.h"
#include "Components/ProgressBar.h"
#include "DragDown.h"

UDDGASStaminaBarUserWidget::UDDGASStaminaBarUserWidget()
{
	PredictionDeltaValue = 5.0f;
	PredictionPeriod = 0.5f;
}

void UDDGASStaminaBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UDDAttributeSet::GetStaminaAttribute()).AddUObject(this, &UDDGASStaminaBarUserWidget::OnStaminaChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UDDAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &UDDGASStaminaBarUserWidget::OnMaxStaminaChanged);
		//UE_LOG(LogCS, Log, TEXT("[NetMode : %d] SetAbilitySystemComponent"), GetWorld()->GetNetMode());
		const UDDAttributeSet* CurrentAttributeSet = ASC->GetSet<UDDAttributeSet>();

		if (CurrentAttributeSet)
		{
			CurrentStamina = CurrentAttributeSet->GetStamina();
			CurrentMaxStamina = CurrentAttributeSet->GetMaxStamina();

			if (CurrentMaxStamina > 0.0f)
			{
				UpdateStaminaBar(CurrentStamina);
			}
			else
			{
				UE_LOG(LogDD, Warning, TEXT("CurrentMaxEnergy is 0"));
			}
		}
		else
		{
			UE_LOG(LogDD, Warning, TEXT("CurrentAttributeSet is null!"));
		}
	}
	else
	{
		UE_LOG(LogDD, Warning, TEXT("ASC is null! Ensure that the Ability System Component is properly initialized before calling this function."));
	}

	if ( !Owner->HasAuthority() )
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaPredictionHandle, this, &UDDGASStaminaBarUserWidget::PredictStaminaUI,
			PredictionPeriod, true, 0.0f);
	}	
}

void UDDGASStaminaBarUserWidget::UpdateStaminaBar(float StaminaToDisplay)
{
	if (PbStaminaBar)
	{
		PbStaminaBar->SetPercent(StaminaToDisplay / CurrentMaxStamina);
	}
}

void UDDGASStaminaBarUserWidget::OnStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	if (CurrentStamina > ChangeData.NewValue)
	{
		InterpolatedStamina = ChangeData.NewValue;
		UpdateStaminaBar(InterpolatedStamina);
	}
	CurrentStamina = ChangeData.NewValue;
}

void UDDGASStaminaBarUserWidget::OnMaxStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxStamina = ChangeData.NewValue;
}

void UDDGASStaminaBarUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!FMath::IsNearlyEqual(InterpolatedStamina, CurrentStamina, 0.01f))
	{
		InterpolatedStamina = FMath::FInterpTo(InterpolatedStamina, CurrentStamina, InDeltaTime, InterpSpeed);
		UpdateStaminaBar(InterpolatedStamina);
	}
}

void UDDGASStaminaBarUserWidget::PredictStaminaUI()
{
	CurrentStamina += PredictionDeltaValue;
}
