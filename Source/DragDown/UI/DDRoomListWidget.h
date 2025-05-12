// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/DDHttpApiSubsystem.h"
#include "DDRoomListWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDRoomListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetRoomList(TArray< FRoomSummary > RoomSummries);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UDDRoomEntryWidget> EntryWidgetToList;
};
