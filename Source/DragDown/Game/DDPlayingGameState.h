// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DDGameState.h"
#include "DDPlayingGameState.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDPlayingGameState : public ADDGameState
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool IsGameEnded() { return bIsGameEnded; };
	FORCEINLINE void SetGameEnded(bool bInGameEnded) { bIsGameEnded = bInGameEnded; };

protected:
	bool bIsGameEnded;
};
