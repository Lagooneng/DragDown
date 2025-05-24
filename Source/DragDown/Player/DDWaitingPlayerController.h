// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DDPlayerController.h"
#include "DDWaitingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDWaitingPlayerController : public ADDPlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void OnChatCallback(const FText& Content) override;

};
