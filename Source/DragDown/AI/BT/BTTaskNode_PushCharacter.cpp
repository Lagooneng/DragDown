// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTaskNode_PushCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Controller/DDAIController.h"
#include "Character/DDCharacterPatrol.h"

UBTTaskNode_PushCharacter::UBTTaskNode_PushCharacter()
{
	NodeName = TEXT("PushCharacter");
}

EBTNodeResult::Type UBTTaskNode_PushCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ADDCharacterPatrol* Patrol = Cast<ADDCharacterPatrol>(OwnerComp.GetAIOwner()->GetPawn());

	if ( Patrol )
	{
		Patrol->PushCharacter();
	}

	return EBTNodeResult::Succeeded;
}
