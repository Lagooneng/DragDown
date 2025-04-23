// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDPlayerController.h"
#include "UI/DDGASStaminabarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "DragDown.h"

ADDPlayerController::ADDPlayerController()
{
	bIsMenuOpen = false;
}

void ADDPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	if (IsLocalController())
	{
		InitGASWidget();
	}
}

void ADDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("OpenMenu", IE_Pressed, this, &ADDPlayerController::ToggleMenu);
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

	StaminaBarWidget->SetOwner(ControlledPawn);
    StaminaBarWidget->SetAbilitySystemComponent(ControlledPawn);

    
}

void ADDPlayerController::ToggleMenu()
{
	if (bIsMenuOpen) CloseMenu();
	else OpenMenu();
}

void ADDPlayerController::OpenMenu()
{
	if (MenuWidgetClass == nullptr) return;
	
	if ( MenuWidget == nullptr )
	{
		MenuWidget = CreateWidget(this, MenuWidgetClass);
	}
	
	MenuWidget->AddToViewport(100);

	FInputModeGameAndUI UIInputMode; 
	UIInputMode.SetWidgetToFocus(MenuWidget->TakeWidget());
	SetInputMode(UIInputMode);
	bShowMouseCursor = true;
	bIsMenuOpen = true;
}

void ADDPlayerController::CloseMenu()
{
	if ( MenuWidget )
	{
		MenuWidget->RemoveFromViewport();
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	bIsMenuOpen = false;
}
