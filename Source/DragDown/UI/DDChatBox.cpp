// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDChatBox.h"
#include "Components/EditableText.h"
#include "Components/VerticalBox.h" 
#include "Components/ScrollBox.h"
#include "UI/DDChatEntry.h"
#include "DragDown.h"

void UDDChatBox::NativeConstruct()
{
	Super::NativeConstruct();

	if ( EditTxtChatInput )
	{
		EditTxtChatInput->OnTextCommitted.AddDynamic(this, &UDDChatBox::OnTextCommitted);
	}
}

void UDDChatBox::MakeChatEntry(const FText& UserName, const FText& Content)
{
	if (ChatEntryClass == nullptr) return;

	UDDChatEntry* ChatEntry = NewObject<UDDChatEntry>(this, ChatEntryClass);
	if (ChatEntry == nullptr) return;
	UE_LOG(LogDD, Log, TEXT("MakeChatEntry"));
	VerticalBox->AddChild(ChatEntry);
	ChatEntry->SetChat(UserName, Content);

	ScrollBox->ScrollToEnd();
}

void UDDChatBox::SetFocusToEditTxt()
{
	if ( EditTxtChatInput )
	{
		EditTxtChatInput->SetFocus();
		EditTxtChatInput->SetKeyboardFocus();
	}
}

void UDDChatBox::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if ( ETextCommit::OnEnter == CommitMethod )
	{
		EditTxtChatInput->SetText(FText::FromString(TEXT("")));
		OnChat.Broadcast(Text);
	}
}
