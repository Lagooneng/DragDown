// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDPeriodicLauncher.h"

// Sets default values
ADDPeriodicLauncher::ADDPeriodicLauncher()
{
	bReplicates = true;

	Offset = 300.0f;
	Rot = FRotator::ZeroRotator;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
}

void ADDPeriodicLauncher::Spawn()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += Offset;
	FRotator SpawnRotation = FRotator(0.f, 90.f, 0.f);

	GetWorld()->SpawnActor<AActor>(ActorClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void ADDPeriodicLauncher::BeginPlay()
{
	Super::BeginPlay();

	if ( HasAuthority() )
	{
		GetWorld()->GetTimerManager().SetTimer(SpawningHandle, this, &ADDPeriodicLauncher::Spawn, 1.0f, true, 0.0f);
	}
}

void ADDPeriodicLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if ( GetWorld() )
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawningHandle);
	}

	Super::EndPlay(EndPlayReason);
}

