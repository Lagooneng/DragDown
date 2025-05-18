// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDPlayerReadyListWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDPlayerReadyListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateReadyList();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDDPlayerReadyEntryWidget> EntryWidgetToList;

	TMap<FString, TObjectPtr<class UDDPlayerReadyEntryWidget>> EntryMap;

	TObjectPtr<class ADDGameState> GameState;
};
