// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DDUserAuthSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDUserAuthSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDDUserAuthSubsystem();
	void ClearSession();

// token
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetToken() { return Token; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetToken(FString InToken) { Token = InToken; }

protected:
	FString Token;

// user info
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetUserName() { return UserName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetUserName(FString InUserName) { UserName = InUserName; }

protected:
	FString UserName;
};
