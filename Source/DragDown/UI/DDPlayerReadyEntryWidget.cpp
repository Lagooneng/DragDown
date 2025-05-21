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

		FSlateColor NewColor = FSlateColor(FLinearColor::Red);
		TxtReady->SetColorAndOpacity(NewColor);
	}
}

void UDDPlayerReadyEntryWidget::UpdateReadyEntry(bool bIsReady)
{
	if ( TxtReady )
	{
		if (bIsReady)
		{
			TxtReady->SetText(FText::FromString(ReadyStatement));

			FSlateColor NewColor = FSlateColor(FLinearColor::Green);
			TxtReady->SetColorAndOpacity(NewColor);
		}
		else
		{
			TxtReady->SetText(FText::FromString(NotReadyStatement));

			FSlateColor NewColor = FSlateColor(FLinearColor::Red);
			TxtReady->SetColorAndOpacity(NewColor);
		}
	}
}
