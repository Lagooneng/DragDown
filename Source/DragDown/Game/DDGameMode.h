// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DDGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPoolInitialized);

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ADDGameMode();
	virtual void BeginPlay() override;
	virtual void StartPlay() override;

	FOnPoolInitialized OnPoolInitialized;

protected:
	// setting in BP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling")
	TObjectPtr<class UDDPooledObjectsData> PooledObjectsData;
};
