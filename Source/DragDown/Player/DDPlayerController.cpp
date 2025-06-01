// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDPlayerController.h"
#include "UI/DDGASStaminabarUserWidget.h"
#include "UI/DDChatBox.h"
#include "AbilitySystemComponent.h"
#include "Game/DDGameState.h"
#include "Subsystem/DDUserAuthSubsystem.h"
#include "DragDown.h"

ADDPlayerController::ADDPlayerController()
{
	bIsMenuOpen = false;
}

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADDPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	if (IsLocalController())
	{
		InitGASWidget();
		InitChatWidget();
	}
}

void ADDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("OpenMenu", IE_Pressed, this, &ADDPlayerController::ToggleMenu);
	InputComponent->BindAction("Chat", IE_Pressed, this, &ADDPlayerController::ActivateChatWidget);
}

void ADDPlayerController::InitGASWidget()
{
	if ( StaminaBarWidgetClass == nullptr || StaminaBarWidget != nullptr) return;

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

	bIsMenuOpen = false;
}

void ADDPlayerController::UpdateChat(const FText& UserName, const FText& Content)
{
	ChatBoxWidget->MakeChatEntry(UserName, Content); 
}

void ADDPlayerController::InitChatWidget()
{
	if (ChatBoxWidgetClass == nullptr || ChatBoxWidget != nullptr) return;

	ChatBoxWidget = CreateWidget<UDDChatBox>(this, ChatBoxWidgetClass);

	if ( ChatBoxWidget == nullptr )
	{
		UE_LOG(LogDD, Log, TEXT("InitGASWidget: StaminaBarWidget Creation Is Failed"));
		return;
	}

	ChatBoxWidget->AddToViewport(); 
	ChatBoxWidget->OnChat.AddDynamic(this, &ADDPlayerController::OnChatCallback);
}

void ADDPlayerController::ActivateChatWidget()
{
	if ( ChatBoxWidget )
	{
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		ChatBoxWidget->SetFocusToEditTxt();
	}
}

void ADDPlayerController::OnChatCallback(const FText& Content)
{
	UE_LOG(LogDD, Log, TEXT("OnChatCallback - %s"), *Content.ToString());

	if (!Content.IsEmpty())
	{
		FText UserName = FText::FromString(GetGameInstance()->GetSubsystem<UDDUserAuthSubsystem>()->GetUserName());

		ServerOnChatCallback(UserName, Content); 
	}
}

void ADDPlayerController::FadeIn(float FadeDuration)
{
	PlayerCameraManager->StartCameraFade(
		1.0f,
		0.0f,
		FadeDuration,
		FColor::Black,
		false,
		true
	);
}

void ADDPlayerController::FadeOut(float FadeDuration)
{
	PlayerCameraManager->StartCameraFade(
		0.0f,
		1.0f,
		FadeDuration,
		FColor::Black,
		false,
		true
	);
}

void ADDPlayerController::ServerOnChatCallback_Implementation(const FText& UserName, const FText& Content)
{
	if ( HasAuthority() )
	{
		ADDGameState* GameState = Cast<ADDGameState>(GetWorld()->GetGameState()); 
		if (GameState) 
		{
			GameState->NetMulticastChatBroadCast(UserName, Content); 
		}
	}
}