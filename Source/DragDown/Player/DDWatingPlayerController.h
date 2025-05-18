// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DDPlayerController.h"
#include "DDWatingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDWatingPlayerController : public ADDPlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

// Ready Setting
public:
	UFUNCTION(BlueprintCallable)
	void SetUserReady(bool bIsReady);

protected:
	void HandleSetUserReady(bool bIsReady);

	UFUNCTION(Server, Reliable)
	void ServerSetUserReady(bool bIsReady);
};
