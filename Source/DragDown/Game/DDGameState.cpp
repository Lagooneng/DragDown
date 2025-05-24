// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DDGameState.h"
#include "Net/UnrealNetwork.h"
#include "Player/DDPlayerState.h"
#include "Player/DDPlayerController.h"
#include "DragDown.h"

void ADDGameState::NetMulticastChatBroadCast_Implementation(const FText& UserName, const FText& Content)
{
	ADDPlayerController* PC = Cast<ADDPlayerController>(GetWorld()->GetFirstPlayerController());
	if ( PC )
	{
		PC->UpdateChat(UserName, Content);
	}
}

void ADDGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	UE_LOG(LogDD, Log, TEXT("[NetMode: %d] ADDGameState::AddPlayerState, %d"), GetWorld()->GetNetMode(), PlayerArray.Num());

	for ( const auto& PS : PlayerArray )
	{
		if ( ADDPlayerState* DDPS = Cast<ADDPlayerState>(PS) )
		{
			DDPS->OnPlayerInfoChanged.Clear();
			DDPS->OnPlayerInfoChanged.AddDynamic(this, &ADDGameState::OnPlayerInfoChangedCallback);
		}
	}
}

void ADDGameState::OnPlayerInfoChangedCallback()
{
	UE_LOG(LogDD, Log, TEXT("ADDGameState::OnPlayerInfoChangedCallback"));
	OnGameInfoChanged.Broadcast();
}
