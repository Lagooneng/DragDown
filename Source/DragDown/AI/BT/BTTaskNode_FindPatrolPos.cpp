// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTaskNode_FindPatrolPos.h"
#include "Character/DDCharacterPatrol.h"
#include "AI/Controller/DDAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DragDown.h"


UBTTaskNode_FindPatrolPos::UBTTaskNode_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTaskNode_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Owner Is Controller
	ADDCharacterPatrol* Patrol = Cast<ADDCharacterPatrol>(OwnerComp.GetAIOwner()->GetPawn());

	if (Patrol)
	{
		Cast<ADDAIController>(OwnerComp.GetAIOwner())->ActiveMove(true);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADDAIController::PatrolPosKey, Patrol->GetPatrolPos());
		return EBTNodeResult::Succeeded;
	}
	else
	{
		UE_LOG(LogDD, Error, TEXT("UBTTaskNode_FindPatrolPos - No Patrol"));
	}

	return EBTNodeResult::Failed;
}
