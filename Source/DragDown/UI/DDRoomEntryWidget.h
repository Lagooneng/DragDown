// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/DDHttpApiSubsystem.h"
#include "DDRoomEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDRoomEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetRoomEntryWidget(FRoomSummary RoomSummary);

	UFUNCTION()
	void JoinRoom();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtRoomName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHostUserName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtPlayerCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnJoinRoom;

	FString RoomID;
};
