// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDChatEntry.h"
#include "Components/TextBlock.h"

void UDDChatEntry::SetChat(const FText& UserName, const FText& Chat)
{
	if ( TxtUserName )
	{
		TxtUserName->SetText(UserName);
	}

	if ( TxtChat )
	{
		TxtChat->SetText(Chat);
	}
}
