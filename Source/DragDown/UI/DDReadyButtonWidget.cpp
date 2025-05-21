// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDReadyButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/DDWaitingPlayerState.h"

void UDDReadyButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsReady = false;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if ( PC )
	{
		WatingPS = PC->GetPlayerState<ADDWaitingPlayerState>();
	}

	if ( BtnReady )
	{
		BtnReady->OnClicked.AddDynamic(this, &UDDReadyButtonWidget::OnReadyButtonClick);
	}

	if ( TxtReady )
	{
		TxtReady->SetText(FText::FromString(NotReadyStatement)); 
	}
}

void UDDReadyButtonWidget::OnReadyButtonClick()
{
	if ( WatingPS )
	{
		bIsReady = !bIsReady;

		WatingPS->SetUserReady(bIsReady);
	}

	if ( TxtReady )
	{
		if ( bIsReady )
		{
			TxtReady->SetText( FText::FromString( ReadyStatement ) );
		}
		else
		{
			TxtReady->SetText(FText::FromString( NotReadyStatement ));
		}
	}
}
