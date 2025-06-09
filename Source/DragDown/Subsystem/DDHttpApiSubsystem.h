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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)	int32 ResponseCode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString ResponseContent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	bool bWasSuccessful;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString ErrorContent;
};

USTRUCT(BlueprintType)
struct FRoomSummary
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString RoomId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString RoomName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString HostUserName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	int32 CurrentPlayerCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	int32 MaxPlayers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	bool bIsGameStarted;
};

USTRUCT(BlueprintType)
struct FRoomDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString RoomId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString RoomName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString HostUserName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString HostIPAddress;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	int32 HostPort;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	int32 MaxPlayers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	TArray<FString> Players;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	TMap<FString, FString> PlayerIPs;	// all users' username -> ipAddress mapped
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	bool bIsGameStarted;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestCompleted, const FResponseStruct&, ResponseData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnListRoomsCompleted, const TArray<FRoomSummary>&, Rooms);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinRoomCompleted, const TArray<FRoomDetails>&, RoomDetails);

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDHttpApiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnRequestCompleted OnRequestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnListRoomsCompleted OnListRoomsCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnJoinRoomCompleted OnJoinRoomCompleted;

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendRegisterRequest(const FString& Username, const FString& Email, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendLoginRequest(const FString& Username, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendCreateRoomRequest(const FString& RoomName);

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendListRoomsRequest();

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendJoinRoomRequest(const FString& RoomID, const FString& InIPAddress, int32 InPort);

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	void SendLeaveRoomRequest();

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	FString GetIP();

	UFUNCTION(BlueprintCallable, Category = "HTTP API")
	FString GetPort();

	void SetAddress();

protected:
	void OnRegisterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnCreateRoomResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnListRoomsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnJoinRoomResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnLeaveRoomResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString	Address;
	FString IP;
	FString Port;

protected:
	FString ServerIP;
	FString ServerPort;
};
