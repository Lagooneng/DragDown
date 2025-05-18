// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DDGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerReadyChanged);

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	FPlayerReadyChanged OnPlayerReadyChanged;

	int32 AddPlayer(const FString& UserName);

	int32 GetPlayerIdx(const FString& UserName);

	const TArray<FString>& GetPlayerNames() { return PlayerNames; }
	const TArray<bool>& GetPlayerReadyStates() { return PlayerReadyStates; }

	void SetPlayerReady(int32 PlayerIdx, bool bIsReady);


protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// TMap is not replicated
// Array Index is Player's Idx
protected:
	UPROPERTY(Replicated)
	TArray< FString > PlayerNames;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerReadyStates)
	TArray< bool > PlayerReadyStates;

	UFUNCTION()
	void OnRep_PlayerReadyStates();
};
