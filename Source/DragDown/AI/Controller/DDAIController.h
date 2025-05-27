// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DDAIController.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ADDAIController();

	virtual void OnPossess(APawn* InPawn) override;

	void ActiveMove(bool bIsActive);

	static const FName PlayerKey;
	static const FName PatrolPosKey;
	static const FName bShouldStopMoveKey;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	TObjectPtr<class UBlackboardData> BBAsset;
};
