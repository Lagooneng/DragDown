// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DDGameMode.h"
#include "Subsystem/DDNetworkObjectPoolingSubsystem.h"
#include "DataAsset/DDPooledObjectsData.h"
#include "DragDown.h"

ADDGameMode::ADDGameMode()
{
}

void ADDGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetSubsystem<UDDNetworkObjectPoolingSubsystem>())
	{
		UDDNetworkObjectPoolingSubsystem* PoolSystem = GetWorld()->GetSubsystem<UDDNetworkObjectPoolingSubsystem>();

		if (PooledObjectsData == nullptr)
		{
			UE_LOG(LogDD, Error, TEXT("No PooledObjectsData"));
			return;
		}
		for (FPooledObjectEntry PooledObjectData : PooledObjectsData->PooledObjects)
		{
			PoolSystem->InitializePool(PooledObjectData.PooledObject, PooledObjectData.PoolingSize);
		}
	}
}

void ADDGameMode::StartPlay()
{
	Super::StartPlay();

	OnPoolInitialized.Broadcast();
}
