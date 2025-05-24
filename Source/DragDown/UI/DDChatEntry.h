// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDChatEntry.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDChatEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetChat(const FText& UserName, const FText& Chat);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtUserName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtChat;
};
