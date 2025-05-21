// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DDPlayerController.h"
#include "DDGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDGamePlayerController : public ADDPlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	virtual void CloseMenu() override;

	void ActivateGameInput();
};
