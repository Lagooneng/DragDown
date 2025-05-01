// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "DDHttpApiSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FResponseStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ResponseCode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ResponseContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWasSuccessful;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ErrorContent;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestCompleted, const FResponseStruct&, ResponseData);

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDHttpApiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnRequestCompleted OnRequestCompleted;

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendRegisterRequest(const FString& Username, const FString& Email, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendLoginRequest(const FString& Username, const FString& Password);

protected:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
