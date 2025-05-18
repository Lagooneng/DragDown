// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DDGameState.h"
#include "Net/UnrealNetwork.h"
#include "DragDown.h"

int32 ADDGameState::AddPlayer(const FString& UserName)
{
	int32 Idx = 0;
	UE_LOG(LogDD, Log, TEXT("ADDGameState - AddPlayer"));

	for ( const FString& PlayerName : PlayerNames )
	{
		++Idx;
		if ( PlayerName == UserName )
		{
			return Idx;
		}
	}

	PlayerNames.Emplace(UserName);
	PlayerReadyStates.Emplace(false);

	OnPlayerReadyChanged.Broadcast();

	return Idx;
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
	if ( PlayerReadyStates.IsValidIndex(PlayerIdx) )
	{
		PlayerReadyStates[PlayerIdx] = bIsReady;
		OnPlayerReadyChanged.Broadcast();
	}
	else
	{
		UE_LOG(LogDD, Error, TEXT("[NetMode: %d] ADDGameState::SetPlayerReady : %d"), GetWorld()->GetNetMode(), PlayerIdx);
	}
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
