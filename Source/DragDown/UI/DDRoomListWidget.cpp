// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDRoomListWidget.h"
#include "UI/DDRoomEntryWidget.h"
#include "Components/VerticalBox.h"
#include "DragDown.h"

void UDDRoomListWidget::SetRoomList(TArray<FRoomSummary> RoomSummries)
{
	VerticalBox->ClearChildren();

	if ( EntryWidgetToList == nullptr )
	{
		UE_LOG(LogDD, Log, TEXT("UDDRoomListWidget::SetRoomList - No Widget To List"));
	}

	if ( VerticalBox == nullptr )
	{
		UE_LOG(LogDD, Log, TEXT("UDDRoomListWidget::SetRoomList - No Vertical Box"));
	}

	for ( FRoomSummary& RoomSummary : RoomSummries )
	{
		UDDRoomEntryWidget* EntryWidget = NewObject<UDDRoomEntryWidget>(this, EntryWidgetToList);
		if (EntryWidget == nullptr)
		{
			UE_LOG(LogDD, Log, TEXT("UDDRoomListWidget::SetRoomList - WidgetToList Creation Failed"));
			return;
		}
		
		VerticalBox->AddChild(EntryWidget);
		EntryWidget->SetRoomEntryWidget(RoomSummary);
		EntryWidget->Padding = 5.0f;
		
	}
}
