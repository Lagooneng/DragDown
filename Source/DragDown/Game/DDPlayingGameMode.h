// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DDGameMode.h"
#include "DDPlayingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDPlayingGameMode : public ADDGameMode
{
	GENERATED_BODY()
	
public:
	ADDPlayingGameMode();

	void EndGame(APlayerState* PS);

protected:
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WinnerMapName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString NonWinnerMapName;

	bool bIsWinnerLogout;
};
