// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DDGameState.h"
#include "DDWaitingGameState.generated.h"


/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDWaitingGameState : public ADDGameState
{
	GENERATED_BODY()
	
public:
	bool AreAllPlayerReady();

protected:

};
