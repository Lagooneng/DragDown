// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDWaitingToGamePortalActor.h"
#include "Game/DDWaitingGameState.h"
#include "UI/DDPromptWidget.h"

void ADDWaitingToGamePortalActor::BeginPlay()
{
	Super::BeginPlay();

	ADDWaitingGameState* GameState = Cast<ADDWaitingGameState>(GetWorld()->GetGameState());
	if ( GameState )
	{
		GameState->OnGameInfoChanged.AddDynamic(this, &ADDWaitingToGamePortalActor::UpdateAndSetDescription);
	}
}

void ADDWaitingToGamePortalActor::Interact(AActor* InterActor)
{
	Super::Interact(InterActor);

	if ( CanInteract(InterActor) )
	{
		GetWorld()->ServerTravel("/Game/02_Level/L_Map_Game?game=/Game/01_Blueprint/Game/BP_DDGameMode.BP_DDGameMode_C?listen");
	}
}

bool ADDWaitingToGamePortalActor::CanInteract(AActor* InterActor)
{
	if ( !InterActor->HasAuthority() )
	{
		return false;
	}

	ADDWaitingGameState* GameState = Cast<ADDWaitingGameState>(GetWorld()->GetGameState());

	if (GameState)
	{
		if (GameState->AreAllPlayerReady())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void ADDWaitingToGamePortalActor::BeginInteract(AActor* InterActor)
{
	UpdateDescription();

	Super::BeginInteract(InterActor);
}

FText ADDWaitingToGamePortalActor::GetInteractionText(AActor* InterActor)
{
	return CurrentDescription;
}

void ADDWaitingToGamePortalActor::UpdateDescription()
{
	if ( !HasAuthority() )
	{
		CurrentDescription = NotHostDescription;
		return;
	}

	ADDWaitingGameState* GameState = Cast<ADDWaitingGameState>(GetWorld()->GetGameState());

	if (GameState)
	{
		if (GameState->AreAllPlayerReady())
		{
			CurrentDescription = StartDescription;
		}
		else
		{
			CurrentDescription = NotReadyDescription;
		}
	}
}

void ADDWaitingToGamePortalActor::UpdateAndSetDescription()
{
	UpdateDescription();
	
	if ( PromptWidget )
	{
		PromptWidget->SetDescription(CurrentDescription);
	}
}
