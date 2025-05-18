// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDPlayerReadyEntryWidget.h"
#include "Components/TextBlock.h"

void UDDPlayerReadyEntryWidget::InitReadyEntry(const FString& UserName)
{
	if ( TxtUserName )
	{
		TxtUserName->SetText( FText::FromString( UserName ) );
	}

	if ( TxtReady )
	{
		TxtReady->SetText(FText::FromString(NotReadyStatement));
	}
}

void UDDPlayerReadyEntryWidget::UpdateReadyEntry(bool bIsReady)
{
	if ( TxtReady )
	{
		if (bIsReady)
		{
			TxtReady->SetText(FText::FromString(ReadyStatement));
		}
		else
		{
			TxtReady->SetText(FText::FromString(NotReadyStatement));
		}
	}
}
