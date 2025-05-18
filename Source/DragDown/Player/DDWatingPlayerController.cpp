// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDWatingPlayerController.h"
#include "Game/DDGameState.h"

void ADDWatingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void ADDWatingPlayerController::SetUserReady(bool bIsReady)
{
	if (HasAuthority())
	{
		HandleSetUserReady(bIsReady);
	}
	else
	{
		ServerSetUserReady(bIsReady);
	}
}

void ADDWatingPlayerController::HandleSetUserReady(bool bIsReady)
{
	if (GameState)
	{
		GameState->SetPlayerReady(PlayerIdx, bIsReady);
	}
}

void ADDWatingPlayerController::ServerSetUserReady_Implementation(bool bIsReady)
{
	HandleSetUserReady(bIsReady);
}

