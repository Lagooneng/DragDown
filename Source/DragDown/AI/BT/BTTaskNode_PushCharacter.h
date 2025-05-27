// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_PushCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UBTTaskNode_PushCharacter : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_PushCharacter();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
