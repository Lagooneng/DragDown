// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DDNetworkObjectPoolingSubsystem.generated.h"

USTRUCT()
struct FActorArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY()
	TArray< TObjectPtr<AActor> > ActorArray;
};


/**
 * Subsystems don't support RPC
 * For Networking, Actors need to use NetMulticast RPCs
 * IPoolable is an interface that triggers the RPC
 */
UCLASS()
class DRAGDOWN_API UDDNetworkObjectPoolingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UDDNetworkObjectPoolingSubsystem();

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void InitializePool(TSubclassOf<AActor> ActorClass, int32 PoolSize);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetPooledObject(TSubclassOf<AActor> ActorClass, const FVector& SpawnLocation, const FRotator& SpawnRotation);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ReturnPooledObject(AActor* Actor);

protected:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActorArrayWrapper > PooledActorsMap;
};
