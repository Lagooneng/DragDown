// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDReadyButtonWidget.h"
#include "Game/DDGameState.h"
#include "Player/DDWatingPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDDReadyButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsReady = false;

	PC = Cast<ADDWatingPlayerController>(GetWorld()->GetFirstPlayerController());

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
	if ( PC )
	{
		bIsReady = !bIsReady;

		PC->SetUserReady(bIsReady);
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
