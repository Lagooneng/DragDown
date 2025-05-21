// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDWaitingPlayerState.h"
#include "Net/UnrealNetwork.h"

ADDWaitingPlayerState::ADDWaitingPlayerState()
{
	bIsUserReady = false;
}

void ADDWaitingPlayerState::SetUserReady(bool bIsReady)
{
	if ( HasAuthority() )
	{
		bIsUserReady = bIsReady;
		OnPlayerInfoChanged.Broadcast();
	}
	else
	{
		ServerSetUserReady(bIsReady);
	}
}

void ADDWaitingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDWaitingPlayerState, bIsUserReady);
}

void ADDWaitingPlayerState::ServerSetUserReady_Implementation(bool bIsReady)
{
	if ( HasAuthority() )
	{
		bIsUserReady = bIsReady;
		OnPlayerInfoChanged.Broadcast(); 
	}
}

void ADDWaitingPlayerState::OnRep_bIsUserReady()
{
	OnPlayerInfoChanged.Broadcast();
}
