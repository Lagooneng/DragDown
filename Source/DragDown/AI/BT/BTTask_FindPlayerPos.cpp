// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FindPlayerPos.h"
#include "Interface/DDPlayerFinderInterface.h"
#include "AI/Controller/DDAIController.h"
#include "Character/DDCharacterPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPlayerPos::UBTTask_FindPlayerPos()
{
	NodeName = TEXT("FindPlayerPos");
}

EBTNodeResult::Type UBTTask_FindPlayerPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IDDPlayerFinderInterface* Finder = Cast<IDDPlayerFinderInterface>(OwnerComp.GetAIOwner()->GetPawn());

	if (Finder)
	{
		ADDCharacterPlayer* Player = Finder->GetCharacterPlayer();

		if (Player)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADDAIController::PlayerKey, Player);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
