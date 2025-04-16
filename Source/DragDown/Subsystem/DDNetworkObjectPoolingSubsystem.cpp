// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DDNetworkObjectPoolingSubsystem.h"
#include "DDNetworkObjectPoolingSubsystem.h"
#include "Interface/DDPoolable.h"
#include "DragDown.h"

UDDNetworkObjectPoolingSubsystem::UDDNetworkObjectPoolingSubsystem()
{
}

void UDDNetworkObjectPoolingSubsystem::InitializePool(TSubclassOf<AActor> ActorClass, int32 PoolSize)
{
    if (!ActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid ActorClass"));
        return;
    }

    if (PooledActorsMap.Contains(ActorClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("ActorClass is already Initalized"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
        return;
    }

    UE_LOG(LogDD, Log, TEXT("InitializePool"));

    TArray<AActor*> ActorPool;
    TSet<AActor*> ActorSet;

    for (int32 i = 0; i < PoolSize; i++)
    {
        AActor* NewActor = World->SpawnActor<AActor>(ActorClass);
        if (NewActor)
        {
            IDDPoolable* PoolableActor = Cast<IDDPoolable>(NewActor);
            if (PoolableActor)
            {
                PoolableActor->OnReturnedToPool();
            }

            ActorPool.Emplace(NewActor);
            ActorSet.Add(NewActor); 
        }
    }

    FActorArrayWrapper Wrapper;
    Wrapper.ActorArray = ActorPool;
    Wrapper.ActorSet = ActorSet;

    PooledActorsMap.Emplace(ActorClass, Wrapper);
}

AActor* UDDNetworkObjectPoolingSubsystem::GetPooledObject(TSubclassOf<AActor> ActorClass, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    if (!PooledActorsMap.Contains(ActorClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("No Actor Pool : %s"), *ActorClass->GetName());
        return nullptr;
    }

    FActorArrayWrapper& ActorPool = PooledActorsMap[ActorClass];
    int32 PoolSize = ActorPool.ActorArray.Num();

    // Like a stack
    if ( PoolSize > 0 )
    {
        AActor* Actor = ActorPool.ActorArray.Last();
        if (Actor == nullptr) return nullptr;
        ActorPool.ActorArray.RemoveAt(PoolSize - 1);
        ActorPool.ActorSet.Remove(Actor);

        Actor->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);

        IDDPoolable* PoolableActor = Cast<IDDPoolable>(Actor);
        if (PoolableActor)
        {
            PoolableActor->OnRetrievedFromPool();
        }

        return Actor;
    }

    // No Actor in Pool -> Spawn
    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* Actor = World->SpawnActor<AActor>(ActorClass, SpawnLocation, SpawnRotation, SpawnParams);

        IDDPoolable* PoolableActor = Cast<IDDPoolable>(Actor);
        if (PoolableActor)
        {
            PoolableActor->OnRetrievedFromPool();
        }

        return Actor;
    }

    return nullptr;
}

void UDDNetworkObjectPoolingSubsystem::ReturnPooledObject(AActor* Actor)
{
    if (!Actor)
    {
        UE_LOG(LogDD, Warning, TEXT("No Actor"));
        return;
    }

    TSubclassOf<AActor> ActorClass = Actor->GetClass();

    if (!PooledActorsMap.Contains(ActorClass))
    {
        UE_LOG(LogDD, Warning, TEXT("No Pool: %s"), *ActorClass->GetName());
        return;
    }

    IDDPoolable* PoolableActor = Cast<IDDPoolable>(Actor);
    if (PoolableActor)
    {
        PoolableActor->OnReturnedToPool();
    }

    FActorArrayWrapper& ActorPool = PooledActorsMap[ActorClass];

    if (!ActorPool.ActorSet.Contains(Actor))
    {
        //UE_LOG(LogTemp, Log, TEXT("Pool Actor : %s"), *ActorClass->GetName());
        ActorPool.ActorArray.Emplace(Actor);
        ActorPool.ActorSet.Add(Actor);
    }
}
