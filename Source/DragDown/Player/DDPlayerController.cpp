// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDPlayerController.h"
#include "UI/DDGASStaminabarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "DragDown.h"

ADDPlayerController::ADDPlayerController()
{
}

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		InitGASWidget();
	}
}

void ADDPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsLocalController())
	{
		InitGASWidget();
	}
}

void ADDPlayerController::InitGASWidget()
{
	UE_LOG(LogDD, Log, TEXT("InitGASWidget Start"));
	if ( StaminaBarWidgetClass == nullptr || StaminaBarWidget != nullptr) return;
	UE_LOG(LogDD, Log, TEXT("InitGASWidget Start - 2"));
	StaminaBarWidget = CreateWidget<UDDGASStaminaBarUserWidget>(this, StaminaBarWidgetClass);

	if (StaminaBarWidget == nullptr)
	{
		UE_LOG(LogDD, Log, TEXT("InitGASWidget: StaminaBarWidget Creation Is Failed"));
		return;
	}

	StaminaBarWidget->AddToViewport();

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        UE_LOG(LogDD, Log, TEXT("InitGASWidget: No Pawn"));
        return;
    }

    StaminaBarWidget->SetAbilitySystemComponent(ControlledPawn);

    
}
