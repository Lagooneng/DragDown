// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DDAttackStateComponent.h"
#include "DataAsset/DDStateDrivenAttackData.h"

// Sets default values for this component's properties
UDDAttackStateComponent::UDDAttackStateComponent()
{
	NowAttackState = 0;

	static ConstructorHelpers::FObjectFinder<UDDStateDrivenAttackData> DataAssetRef(TEXT("/Script/DragDown.DDStateDrivenAttackData'/Game/Blueprint/DataAsset/DDDA_DirevenAttackData.DDDA_DirevenAttackData'"));

	if ( DataAssetRef.Succeeded() )
	{
		StateDrivenAttackData = DataAssetRef.Object;
		MaxAttackState = DataAssetRef.Object.Get()->MaxStateCount;
	}
}

FString UDDAttackStateComponent::GetSectionPrefix()
{
	if (StateDrivenAttackData) return StateDrivenAttackData->MontageSectionNamePrefix;
	return FString();
}

float UDDAttackStateComponent::GetPower()
{
	if (StateDrivenAttackData) return StateDrivenAttackData->AttackPower[NowAttackState];
	return 0.0f;
}

float UDDAttackStateComponent::GetZPower()
{
	if (StateDrivenAttackData) return StateDrivenAttackData->ZPower[NowAttackState];
	return 0.0f;
}

float UDDAttackStateComponent::GetNecessaryStamina()
{
	if (StateDrivenAttackData) return StateDrivenAttackData->NecessaryStamina[NowAttackState];
	return 0.0f;
}

