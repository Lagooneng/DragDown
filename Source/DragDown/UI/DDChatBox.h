// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDChatBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatDelegate, const FText&, Content);

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDChatBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void MakeChatEntry(const FText& UserName, const FText& Content);

	FChatDelegate OnChat;

	void SetFocusToEditTxt();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ScrollBox; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDDChatEntry> ChatEntryClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableText> EditTxtChatInput;

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
