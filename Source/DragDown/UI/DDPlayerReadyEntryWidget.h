// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDPlayerReadyEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDPlayerReadyEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitReadyEntry(const FString& UserName);
	void UpdateReadyEntry(bool bIsReady);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtUserName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtReady;

	const FString ReadyStatement = "Ready";
	const FString NotReadyStatement = "Not Ready"; 
};
