// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DDGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameInfoChanged);

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	FGameInfoChanged OnGameInfoChanged;

protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	UFUNCTION()
	void OnPlayerInfoChangedCallback();
};
