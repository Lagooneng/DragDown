// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDPlayerReadyListWidget.h"
#include "Game/DDGameState.h"
#include "Player/DDWaitingPlayerState.h"
#include "UI/DDPlayerReadyEntryWidget.h"
#include "Components/VerticalBox.h"
#include "DragDown.h"

void UDDPlayerReadyListWidget::NativeConstruct()
{
	UE_LOG(LogDD, Log, TEXT("[NetMode: %d] NativeConstruct"), GetWorld()->GetNetMode());
	GameState = Cast<ADDGameState>(GetWorld()->GetGameState());

	if ( GameState )
	{
		GameState->OnGameInfoChanged.AddDynamic(this, &UDDPlayerReadyListWidget::UpdateReadyList);
	}

	if ( !GetWorld()->GetFirstPlayerController()->HasAuthority() )
	{
		UpdateReadyList();
	}
	//UpdateReadyList();
}

void UDDPlayerReadyListWidget::UpdateReadyList()
{
	if ( VerticalBox == nullptr ) return;
	if ( GameState == nullptr ) return;
	
	for ( const auto& PS : GameState->PlayerArray )
	{
		ADDWaitingPlayerState* WaitingDDPS = Cast<ADDWaitingPlayerState>(PS);
		if (WaitingDDPS == nullptr) return;

		if ( EntryMap.Contains( WaitingDDPS->GetUserName() ) )
		{
			bool bIsReady = WaitingDDPS->IsUserReady();
			EntryMap[WaitingDDPS->GetUserName()]->UpdateReadyEntry(bIsReady);
		}
		else
		{
			if (EntryWidgetToList == nullptr)
			{
				UE_LOG(LogDD, Log, TEXT("UDDPlayerReadyListWidget - EntryWidgetToList Is Null"));
				return;
			}

			UDDPlayerReadyEntryWidget* EntryWidget = NewObject<UDDPlayerReadyEntryWidget>(this, EntryWidgetToList);
			if (EntryWidget == nullptr) return;

			VerticalBox->AddChild(EntryWidget);
			EntryWidget->InitReadyEntry(WaitingDDPS->GetUserName());

			EntryMap.Emplace(WaitingDDPS->GetUserName(), EntryWidget);
		}
	}
}
