// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DDAttackStateComponent.h"
#include "DataAsset/DDStateDrivenAttackData.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "DragDown.h"

// Sets default values for this component's properties
UDDAttackStateComponent::UDDAttackStateComponent()
{
	SetIsReplicatedByDefault(true);

	NowAttackState = 0;
	MaxAttackState = 0;
}

void UDDAttackStateComponent::BeginPlay()
{
	Super::BeginPlay();

	if (StateDrivenAttackData)
	{
		MaxAttackState = StateDrivenAttackData->MaxStateCount;
	}
}

void UDDAttackStateComponent::PlusAttackState()
{
	if (MaxAttackState == 0)
	{
		UE_LOG(LogDD, Error, TEXT("UDDAttackStateComponent::PlusAttackState - Divide by Zero"));
	}

	NowAttackState = (NowAttackState + 1) % MaxAttackState;
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
	UE_LOG(LogDD, Log, TEXT("[NetMode : %d] NowAttackState : %d"), GetWorld()->GetNetMode(), NowAttackState);

	if (StateDrivenAttackData) return StateDrivenAttackData->NecessaryStamina[NowAttackState];
	return 0.0f;
}

void UDDAttackStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDDAttackStateComponent, NowAttackState);
}

