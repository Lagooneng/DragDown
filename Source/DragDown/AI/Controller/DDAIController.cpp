// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/DDAIController.h"
#include "BehaviorTree/BehaviorTree.h" 
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DragDown.h"

const FName ADDAIController::PlayerKey(TEXT("Player"));
const FName ADDAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ADDAIController::bShouldStopMoveKey(TEXT("bShouldStopMove"));
 
ADDAIController::ADDAIController()
{
}

void ADDAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UBlackboardComponent* BBComponent = Blackboard;

	if (UseBlackboard(BBAsset, BBComponent))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogDD, Log, TEXT("RunBehaviorTree Failed"));
		}
	}
}

void ADDAIController::ActiveMove(bool bIsActive)
{
	UBlackboardComponent* BBComponent = GetBlackboardComponent();

	if (BBComponent)
	{
		BBComponent->SetValueAsBool(bShouldStopMoveKey, !bIsActive);
	}
}
