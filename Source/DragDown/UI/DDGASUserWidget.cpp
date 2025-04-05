// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDGASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UDDGASUserWidget::SetOwner(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		Owner = InOwner;
	}
}

void UDDGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UDDGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
