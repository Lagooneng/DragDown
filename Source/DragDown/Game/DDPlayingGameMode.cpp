// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DDPlayingGameMode.h"
#include "Game/DDPlayingGameState.h"
#include "GameFramework/PlayerState.h"
#include "DDPlayingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DragDown.h"

ADDPlayingGameMode::ADDPlayingGameMode()
{
	bIsWinnerLogout = false;
}

void ADDPlayingGameMode::EndGame(APlayerState* PS)
{
	ADDPlayingGameState* GS = Cast<ADDPlayingGameState>( GetWorld()->GetGameState() );

	if ( GS->PlayerArray.Num() == 1 )
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*WinnerMapName)); 
		return;
	}

	
	GS->SetGameEnded(true);
	
	for ( APlayerState* PlayerState : GS->PlayerArray )
	{
		APlayerController* PC = PlayerState->GetPlayerController();

		if ( PC && !PC->IsLocalController() )	// For Non-Server, Because GameMode is Server Only Actor
		{
			UE_LOG(LogDD, Log, TEXT("ADDPlayingGameMode::EndGame %s"), *PlayerState->GetName());

			if ( PlayerState == PS )
			{
				bIsWinnerLogout = true;
				PC->ClientTravel(WinnerMapName, ETravelType::TRAVEL_Absolute);
			}
			else
			{
				PC->ClientTravel(NonWinnerMapName, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void ADDPlayingGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ADDPlayingGameState* GS = Cast<ADDPlayingGameState>(GetWorld()->GetGameState()); 

	if ( GS && GS->PlayerArray.Num() - 1 == 1 )
	{
		// Only Server Remain
		if ( !bIsWinnerLogout )
		{
			UGameplayStatics::OpenLevel(GetWorld(), FName(*WinnerMapName));
		}
		else
		{
			UE_LOG(LogDD, Log, TEXT("Server is not winner"));
			UGameplayStatics::OpenLevel(GetWorld(), FName(*NonWinnerMapName));
		}
	}
}
