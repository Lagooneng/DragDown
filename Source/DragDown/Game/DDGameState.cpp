// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DDGameState.h"
#include "Net/UnrealNetwork.h"
#include "DragDown.h"

void ADDGameState::AddPlayer(const FString& UserName)
{
	UE_LOG(LogDD, Log, TEXT("ADDGameState - AddPlayer"));

	for ( const FString& PlayerName : PlayerNames )
	{
		if ( PlayerName == UserName )
		{
			return;
		}
	}

	PlayerNames.Emplace(UserName);
	PlayerReadyStates.Emplace(false);

	OnPlayerReadyChanged.Broadcast();
}

int32 ADDGameState::GetPlayerIdx(const FString& UserName)
{
	int32 Length = PlayerNames.Num();

	for (int32 i = 0; i < Length; ++i)
	{
		if ( PlayerNames[i] == UserName)
		{
			return i;
		}
	}

	return -1;
}

void ADDGameState::SetPlayerReady(int32 PlayerIdx, bool bIsReady)
{
	PlayerReadyStates[PlayerIdx] = bIsReady;

	OnPlayerReadyChanged.Broadcast();
}

void ADDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDGameState, PlayerNames);
	DOREPLIFETIME(ADDGameState, PlayerReadyStates);
}

void ADDGameState::OnRep_PlayerReadyStates()
{
	UE_LOG(LogDD, Log, TEXT("[NetMode: %d] OnRep_PlayerReadyStruct"), GetWorld()->GetNetMode());
	OnPlayerReadyChanged.Broadcast();
}
