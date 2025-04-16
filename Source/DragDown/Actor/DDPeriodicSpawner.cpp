// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDPeriodicSpawner.h"
#include "Subsystem/DDNetworkObjectPoolingSubsystem.h"

// Sets default values
ADDPeriodicSpawner::ADDPeriodicSpawner()
{
	bReplicates = true;

	Offset = 150.0f;
	SpawningRot = FRotator::ZeroRotator;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
}

void ADDPeriodicSpawner::Spawn()
{
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += Offset;
	FRotator SpawnRotation = SpawningRot;

	if ( GetWorld() )
	{
		UDDNetworkObjectPoolingSubsystem* PoolingSystem = GetWorld()->GetSubsystem<UDDNetworkObjectPoolingSubsystem>();

		PoolingSystem->GetPooledObject(ActorClass, SpawnLocation, SpawnRotation);
	}
}

void ADDPeriodicSpawner::BeginPlay()
{
	Super::BeginPlay();

	if ( HasAuthority() )
	{
		GetWorld()->GetTimerManager().SetTimer(SpawningHandle, this, &ADDPeriodicSpawner::Spawn, 1.0f, true, 0.0f);
	}
}

void ADDPeriodicSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if ( GetWorld() )
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawningHandle);
	}

	Super::EndPlay(EndPlayReason);
}

