// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDWaitingPlayerController.h"
#include "Game/DDGameState.h"

void ADDWaitingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void ADDWaitingPlayerController::OnChatCallback(const FText& Content)
{
	Super::OnChatCallback(Content);

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

