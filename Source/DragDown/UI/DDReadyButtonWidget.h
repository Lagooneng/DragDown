// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDReadyButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDReadyButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReadyButtonClick();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnReady;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtReady;

	UPROPERTY()
	TObjectPtr<class ADDWatingPlayerController> PC;

	bool bIsReady;

	const FString ReadyStatement = "Ready";
	const FString NotReadyStatement = "Not Ready";
};
