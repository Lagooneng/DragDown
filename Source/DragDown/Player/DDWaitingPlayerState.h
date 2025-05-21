// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DDPlayerState.h"
#include "DDWaitingPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDWaitingPlayerState : public ADDPlayerState
{
	GENERATED_BODY()
	
public:
	ADDWaitingPlayerState();

	FORCEINLINE bool IsUserReady() { return bIsUserReady; }

	void SetUserReady(bool bIsReady);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerSetUserReady(bool bIsReady);

	UPROPERTY(ReplicatedUsing = OnRep_bIsUserReady)
	bool bIsUserReady;

	UFUNCTION()
	void OnRep_bIsUserReady();
};
