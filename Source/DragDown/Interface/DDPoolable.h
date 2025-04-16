// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DDPoolable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DRAGDOWN_API IDDPoolable
{
	GENERATED_BODY()

public:
	/**
	* Usage:
	* void ClassName::OnRetrievedFromPool()
	{
		if ( HasAuthority() )
		{
			~~~ Timer Setting (if need) ~~~, timer needs to be activated in server, not client

			NetMulticastOnRetrievedFromPool(GetActorLocation(), GetActorRotation()); // for multi sync
		}
	}
	void ClassName::NetMulticastOnRetrievedFromPool_Implementation(FVector NewLocation, FRotator NewRotation)
	{
		~~~ Specific Setting (if need) ~~~

		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);

		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
	*/
	virtual void OnRetrievedFromPool() = 0;

	/*
	* void ClassName::OnReturnedToPool()
	{
		if ( HasAuthority() )
		{
			NetMulticastOnReturnedToPool(); // for multi sync
		}
	}
	void ClassName::NetMulticastOnReturnedToPool_Implementation()
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
	*/
	virtual void OnReturnedToPool() = 0;
};
