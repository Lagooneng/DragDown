// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDPlayerController.h"
#include "UI/DDGASStaminabarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Game/DDGameState.h"
#include "Subsystem/DDUserAuthSubsystem.h"
#include "DragDown.h"

ADDPlayerController::ADDPlayerController()
{
	bIsMenuOpen = false;
	PlayerIdx = -1;
}

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if ( GameState == nullptr )
	{
		GameState = Cast<ADDGameState>(GetWorld()->GetGameState());
	}

}

void ADDPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	if (IsLocalController())
	{
		if (GameState == nullptr)
		{
			GameState = Cast<ADDGameState>(GetWorld()->GetGameState());
		}

		InitGASWidget();
		SetUserName();
	}
}

void ADDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("OpenMenu", IE_Pressed, this, &ADDPlayerController::ToggleMenu);
}

void ADDPlayerController::HandleSetUserName(const FString& InUserName)
{
	if ( !HasAuthority() ) return;

	if ( GameState )
	{
		PlayerIdx = GameState->AddPlayer(InUserName); 
		UE_LOG(LogDD, Log, TEXT("[NetMode: %d] ADDPlayerController::HandleSetUserName - %s, %d"), GetWorld()->GetNetMode(), *InUserName, PlayerIdx);
	}
	else
	{
		UE_LOG(LogDD, Error, TEXT("[NetMode: %d] GameState is null"), GetWorld()->GetNetMode());
	}
}

void ADDPlayerController::SetUserName()
{
	UDDUserAuthSubsystem* UserAuthSubsystem = GetGameInstance()->GetSubsystem<UDDUserAuthSubsystem>();

	if ( HasAuthority() )
	{
		HandleSetUserName(UserAuthSubsystem->GetUserName());
	}
	else
	{
		ServerSetUserName(UserAuthSubsystem->GetUserName());
	}
}

void ADDPlayerController::ServerSetUserName_Implementation(const FString& InUserName)
{
	if (HasAuthority())
	{
		HandleSetUserName(InUserName);
	}
}

void ADDPlayerController::InitGASWidget()
{
	//UE_LOG(LogDD, Log, TEXT("InitGASWidget Start"));
	if ( StaminaBarWidgetClass == nullptr || StaminaBarWidget != nullptr) return;
	//UE_LOG(LogDD, Log, TEXT("InitGASWidget Start - 2"));
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
	
	MenuWidget = CreateWidget(this, MenuWidgetClass);
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
		MenuWidget->RemoveFromParent();
	}

	/*FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;*/
	bIsMenuOpen = false;
}
