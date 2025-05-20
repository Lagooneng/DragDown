// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DDWaitingGameState.h"
#include "Player/DDWaitingPlayerState.h"

bool ADDWaitingGameState::AreAllPlayerReady()
{
	if (!HasAuthority()) return false;

	for (const auto& PS : PlayerArray)
	{
		ADDWaitingPlayerState* WaitingPS = Cast<ADDWaitingPlayerState>(PS);
		if (!WaitingPS->IsUserReady())
		{
			return false;
		}
	}

	return true;
}