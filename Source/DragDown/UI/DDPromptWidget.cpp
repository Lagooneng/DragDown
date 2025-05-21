// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDPromptWidget.h"
#include "Components/TextBlock.h"

void UDDPromptWidget::SetDescription(const FText& InDescription)
{
	if ( Description )
	{
		Description->SetText(InDescription);
	}
}
