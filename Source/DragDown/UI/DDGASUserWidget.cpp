// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDGASUserWidget.h"
#include "ActorComponent/DDGASManagerComponent.h"

void UDDGASUserWidget::SetOwner(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		Owner = InOwner;
	}
}

void UDDGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (!IsValid(InOwner)) return;

	UDDGASManagerComponent* GASManagerComponent = InOwner->FindComponentByClass<UDDGASManagerComponent>();
	if (!IsValid(GASManagerComponent)) return;

	ASC = InOwner->FindComponentByClass<UDDGASManagerComponent>()->GetAbilitySystemComponent();
	
}

UAbilitySystemComponent* UDDGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
