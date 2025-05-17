// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DDGameState.generated.h"

USTRUCT()
struct FPlayerReadyStruct
{
	GENERATED_BODY()

	TArray<FString> PlayerNames;

	TMap<FString, bool> PlayerReadyStates;
};

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	void AddPlayer(const FString& UserName);

	void SetPlayerReady(const FString& UserName, bool bIsReady);

	FORCEINLINE FPlayerReadyStruct GetPlayerReadyStruct() { return PlayerReadyStruct; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	FPlayerReadyStruct PlayerReadyStruct;
};
