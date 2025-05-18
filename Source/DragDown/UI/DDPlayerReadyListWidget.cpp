// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDPlayerReadyListWidget.h"
#include "Game/DDGameState.h"
#include "UI/DDPlayerReadyEntryWidget.h"
#include "Components/VerticalBox.h"
#include "DragDown.h"

void UDDPlayerReadyListWidget::NativeConstruct()
{
	GameState = Cast<ADDGameState>(GetWorld()->GetGameState());

	UE_LOG(LogDD, Log, TEXT("UDDPlayerReadyListWidget NativeConstruct"));
	if ( GameState )
	{
		GameState->OnPlayerReadyChanged.AddDynamic(this, &UDDPlayerReadyListWidget::UpdateReadyList);
	}

	UpdateReadyList();
}

void UDDPlayerReadyListWidget::UpdateReadyList()
{
	if ( VerticalBox == nullptr ) return;
	if ( GameState == nullptr ) return;
	
	const TArray<FString>& PlayerNames = GameState->GetPlayerNames();
	const TArray<bool>& PlayerReadyStates = GameState->GetPlayerReadyStates();
	
	int Length = PlayerNames.Num();

	for (int32 i = 0; i < Length; i++)
	{
		if (EntryMap.Contains(PlayerNames[i]))
		{
			bool bIsReady = PlayerReadyStates[i];
			EntryMap[ PlayerNames[i] ]->UpdateReadyEntry(bIsReady);
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
			EntryWidget->InitReadyEntry( PlayerNames[i] );

			EntryMap.Emplace(PlayerNames[i], EntryWidget);
		}
	}
}
