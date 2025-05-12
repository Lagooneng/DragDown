// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDRoomEntryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DragDown.h"

void UDDRoomEntryWidget::NativeConstruct()
{
	if (BtnJoinRoom)
	{
		BtnJoinRoom->OnClicked.AddDynamic(this, &UDDRoomEntryWidget::JoinRoom);
	}
}

void UDDRoomEntryWidget::SetRoomEntryWidget(FRoomSummary RoomSummary)
{
	RoomID = RoomSummary.RoomId;

	if (TxtRoomName)
	{
		TxtRoomName->SetText(FText::FromString(RoomSummary.RoomName ));
	}

	if (TxtHostUserName)
	{
		TxtHostUserName->SetText(FText::FromString(RoomSummary.HostUserName));
	}

	if ( TxtPlayerCount )
	{
		FString Count = FString::Printf(TEXT("%d / %d"), RoomSummary.CurrentPlayerCount, RoomSummary.MaxPlayers);
		TxtPlayerCount->SetText(FText::FromString(Count));
	}

	UE_LOG(LogDD, Log, TEXT("Room: %s, Host: %s, Players %d - %d"),
		*RoomSummary.RoomName,
		*RoomSummary.HostUserName,
		RoomSummary.CurrentPlayerCount,
		RoomSummary.MaxPlayers);
}

void UDDRoomEntryWidget::JoinRoom()
{
	UDDHttpApiSubsystem* HttpSubsystem = GetGameInstance()->GetSubsystem<UDDHttpApiSubsystem>();
	if (HttpSubsystem == nullptr)
	{
		UE_LOG(LogDD, Error, TEXT("No UDDHttpApiSubsystem"));
	}

	FString IP = HttpSubsystem->GetIP();
	int32 Port = FCString::Atoi( *HttpSubsystem->GetPort() );

	HttpSubsystem->SendJoinRoomRequest(RoomID, IP, Port);
}
