// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "DDHttpApiSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDHttpApiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendRegisterRequest(const FString& Username, const FString& Email, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendLoginRequest(const FString& Username, const FString& Password);

protected:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
