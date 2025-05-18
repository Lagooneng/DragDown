// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDGamePlayerController.h"
#include "DDGamePlayerController.h"

void ADDGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	ActivateGameInput();
}

void ADDGamePlayerController::CloseMenu()
{
	Super::CloseMenu();

	ActivateGameInput();
}

void ADDGamePlayerController::ActivateGameInput()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode); 
	bShowMouseCursor = false; 
}
