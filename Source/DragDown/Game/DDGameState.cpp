// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DDGameState.h"
#include "Net/UnrealNetwork.h"
#include "DragDown.h"

void ADDGameState::AddPlayer(const FString& UserName)
{
	if ( !PlayerReadyStruct.PlayerNames.Find(UserName) )
	{
		PlayerReadyStruct.PlayerNames.Emplace(UserName); 
	}

	if ( !PlayerReadyStruct.PlayerReadyStates.Find(UserName) )
	{
		PlayerReadyStruct.PlayerReadyStates.Emplace(UserName, false);
	}
}

void ADDGameState::SetPlayerReady(const FString& UserName, bool bIsReady)
{
	if ( PlayerReadyStruct.PlayerReadyStates.Find(UserName) )
	{
		PlayerReadyStruct.PlayerReadyStates[UserName] = bIsReady;
	}
}

void ADDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDGameState, PlayerReadyStruct);
}
