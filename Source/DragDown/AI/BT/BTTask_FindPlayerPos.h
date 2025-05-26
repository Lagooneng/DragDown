// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPlayerPos.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UBTTask_FindPlayerPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPlayerPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
