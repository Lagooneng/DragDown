// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDGASStaminaBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/DDAttributeSet.h"
#include "Components/ProgressBar.h"
#include "DragDown.h"

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
				UpdateStaminaBar();
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
}

void UDDGASStaminaBarUserWidget::UpdateStaminaBar()
{
	if (PbStaminaBar)
	{
		UE_LOG(LogDD, Log, TEXT("UpdateEnergyBar : %f / %f"), CurrentStamina, CurrentMaxStamina); 
		PbStaminaBar->SetPercent(CurrentStamina / CurrentMaxStamina);
	}
}

void UDDGASStaminaBarUserWidget::OnStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	UE_LOG(LogDD, Log, TEXT("[NetMode %d]OnEnergyChanged"), GetWorld()->GetNetMode());
	CurrentStamina = ChangeData.NewValue;
	UpdateStaminaBar();
}

void UDDGASStaminaBarUserWidget::OnMaxStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxStamina = ChangeData.NewValue;
	UpdateStaminaBar();
}
