// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDGASWidgetComponent.h"
#include "UI/DDGASUserWidget.h"
#include "DragDown.h"

void UDDGASWidgetComponent::ActivateGAS()
{
	UDDGASUserWidget* GASUserWidget = Cast<UDDGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
	else
	{
		UE_LOG(LogDD, Log, TEXT("ActivateGAS Failed - No GASUserWidget"));
	}
}

void UDDGASWidgetComponent::InitWidget()
{
	Super::InitWidget();
	//UE_LOG(LogCS, Log, TEXT("InitWidget IS Called"));
	UDDGASUserWidget* GASUserWidget = Cast<UDDGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetOwner(GetOwner());
	}
	else
	{
		UE_LOG(LogDD, Log, TEXT("InitWidget Failed - No GASUserWidget"));
	}
}
