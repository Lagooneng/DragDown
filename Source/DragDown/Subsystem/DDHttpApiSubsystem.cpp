// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DDHttpApiSubsystem.h"
#include "Subsystem/DDUserAuthSubsystem.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "HttpModule.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "DragDown.h"

void UDDHttpApiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SetAddress();
}

void UDDHttpApiSubsystem::SendRegisterRequest(const FString& Username, const FString& Email, const FString& Password)
{
	FString Url = TEXT("http://localhost:8080/api/auth/register");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("username"), Username);
	JsonObject->SetStringField(TEXT("email"), Email);
	JsonObject->SetStringField(TEXT("password"), Password);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString); 
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer); 

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(OutputString);
	Request->OnProcessRequestComplete().BindUObject(this, &UDDHttpApiSubsystem::OnRegisterResponseReceived);
	Request->ProcessRequest();
}

void UDDHttpApiSubsystem::SendLoginRequest(const FString& Username, const FString& Password)
{
	FString Url = TEXT("http://localhost:8080/api/auth/login");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("username"), Username);
	JsonObject->SetStringField(TEXT("password"), Password);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(OutputString);
	Request->OnProcessRequestComplete().BindUObject(this, &UDDHttpApiSubsystem::OnLoginResponseReceived);
	Request->ProcessRequest();
}

void UDDHttpApiSubsystem::SendCreateRoomRequest(const FString& RoomName)
{
	FString Url = TEXT("http://localhost:8080/api/MatchRooms");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("roomName"), RoomName);
	JsonObject->SetStringField(TEXT("ipAddress"), IP);
	JsonObject->SetStringField(TEXT("port"), Port);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader("Content-Type", "application/json");

	UE_LOG(LogDD, Log, TEXT("Token: %s"), *GetGameInstance()->GetSubsystem<UDDUserAuthSubsystem>()->GetToken());
	FString RawToken = GetGameInstance()->GetSubsystem<UDDUserAuthSubsystem>()->GetToken();
	FString Bearer = FString::Printf(TEXT("Bearer %s"), *RawToken);
	Request->SetHeader("Authorization", Bearer);
	Request->SetContentAsString(OutputString); 
	Request->OnProcessRequestComplete().BindUObject(this, &UDDHttpApiSubsystem::OnCreateRoomResponseReceived);
	Request->ProcessRequest(); 
}

void UDDHttpApiSubsystem::SendListRoomsRequest()
{
	FString Url = TEXT("http://localhost:8080/api/MatchRooms");
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader("Content-Type", "application/json");

	FString RawToken = GetGameInstance()->GetSubsystem<UDDUserAuthSubsystem>()->GetToken();
	FString Bearer = FString::Printf(TEXT("Bearer %s"), *RawToken);
	Request->SetHeader("Authorization", Bearer);

	Request->OnProcessRequestComplete().BindUObject(this, &UDDHttpApiSubsystem::OnListRoomsResponseReceived);
	Request->ProcessRequest();
}

void UDDHttpApiSubsystem::SendJoinRoomRequest(const FString& RoomID, const FString& InIPAddress, int32 InPort)
{
	FString Url = FString::Printf(TEXT("http://localhost:8080/api/MatchRooms/%s/join"), *RoomID);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader("Content-Type", "application/json");

	FString RawToken = GetGameInstance()->GetSubsystem<UDDUserAuthSubsystem>()->GetToken();
	FString Bearer = FString::Printf(TEXT("Bearer %s"), *RawToken);
	Request->SetHeader("Authorization", Bearer);

	TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject());
	Json->SetStringField(TEXT("ipAddress"), InIPAddress);
	Json->SetNumberField(TEXT("port"), InPort);

	FString Body;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Body);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
	Request->SetContentAsString(Body);

	Request->OnProcessRequestComplete().BindUObject(this, &UDDHttpApiSubsystem::OnJoinRoomResponseReceived);
	Request->ProcessRequest();
}

FString UDDHttpApiSubsystem::GetIP()
{
	return IP;
}

void UDDHttpApiSubsystem::OnRegisterResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FResponseStruct ResponseStruct;
	ResponseStruct.bWasSuccessful = bWasSuccessful;

	if (bWasSuccessful && Response.IsValid())
	{
		int32 ResponseCode = Response->GetResponseCode();
		FString ResponseContent = Response->GetContentAsString();
		ResponseStruct.ResponseCode = ResponseCode;
		ResponseStruct.ResponseContent = ResponseContent;

		UE_LOG(LogDD, Log, TEXT("ResponseCode: %d"), ResponseCode);
		UE_LOG(LogDD, Log, TEXT("ResponseContent: %s"), *ResponseContent);
	}
	else
	{
		FString Error = Response.IsValid() ? Response->GetContentAsString() : TEXT("Invalid Response");
		ResponseStruct.ErrorContent = Error;

		UE_LOG(LogDD, Error, TEXT("Request Failed: %s"), *Error);
	}

	// UnBind는 바인딩하는 쪽에서 알아서
	OnRequestCompleted.Broadcast(ResponseStruct);
}

void UDDHttpApiSubsystem::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FResponseStruct ResponseStruct;
	ResponseStruct.bWasSuccessful = bWasSuccessful;

	if (bWasSuccessful && Response.IsValid())
	{
		int32 ResponseCode = Response->GetResponseCode();
		FString ResponseContent = Response->GetContentAsString();
		ResponseStruct.ResponseCode = ResponseCode;
		ResponseStruct.ResponseContent = ResponseContent;

		UE_LOG(LogDD, Log, TEXT("ResponseCode: %d"), ResponseCode);
		UE_LOG(LogDD, Log, TEXT("ResponseContent: %s"), *ResponseContent);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

		if ( FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid() )
		{
			FString Token = JsonObject->GetStringField(TEXT("token"));
			GetGameInstance()->GetSubsystem<UDDUserAuthSubsystem>()->SetToken(Token);
		}
	}
	else
	{
		FString Error = Response.IsValid() ? Response->GetContentAsString() : TEXT("Invalid Response");
		ResponseStruct.ErrorContent = Error;

		UE_LOG(LogDD, Error, TEXT("Request Failed: %s"), *Error);
	}

	// UnBind는 바인딩하는 쪽에서 알아서
	OnRequestCompleted.Broadcast(ResponseStruct);
}

void UDDHttpApiSubsystem::OnCreateRoomResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FResponseStruct ResponseStruct;
	ResponseStruct.bWasSuccessful = bWasSuccessful;

	if (bWasSuccessful && Response.IsValid())
	{
		int32 ResponseCode = Response->GetResponseCode();
		FString ResponseContent = Response->GetContentAsString();
		ResponseStruct.ResponseCode = ResponseCode;
		ResponseStruct.ResponseContent = ResponseContent;

		UE_LOG(LogDD, Log, TEXT("ResponseCode: %d"), ResponseCode);
		UE_LOG(LogDD, Log, TEXT("ResponseContent: %s"), *ResponseContent);

		GetWorld()->ServerTravel(TEXT("/Game/02_Level/L_WatingRoom?game=/Game/01_Blueprint/Game/BP_DDWatingMode.BP_DDWatingMode_C?listen"));
	} 
	else
	{
		FString Error = Response.IsValid() ? Response->GetContentAsString() : TEXT("Invalid Response");
		ResponseStruct.ErrorContent = Error;

		UE_LOG(LogDD, Error, TEXT("Request Failed: %s"), *Error);
	}

	// UnBind는 바인딩하는 쪽에서 알아서
	OnRequestCompleted.Broadcast(ResponseStruct);
}

void UDDHttpApiSubsystem::OnListRoomsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TArray<FRoomSummary> RoomSummaries;

	if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == 200)
	{
		UE_LOG(LogDD, Log, TEXT("%s"), *Response->GetContentAsString());

		FString JsonString = Response->GetContentAsString();
		TSharedPtr<FJsonValue> JsonValue;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if ( FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid() && JsonValue->AsArray().Num() > 0)
		{
			const TArray<TSharedPtr<FJsonValue>>& Arr = JsonValue->AsArray();

			for (auto& Entry : Arr)
			{
				const TSharedPtr<FJsonObject>& Obj = Entry->AsObject();
				if (!Obj.IsValid()) return;

				FRoomSummary Info;
				Info.RoomId = Obj->GetStringField(TEXT("roomId"));
				Info.RoomName = Obj->GetStringField(TEXT("roomName"));
				Info.HostUserName = Obj->GetStringField(TEXT("hostUsername"));
				Info.CurrentPlayerCount = Obj->GetIntegerField(TEXT("currentPlayerCount"));
				Info.MaxPlayers = Obj->GetIntegerField(TEXT("maxPlayers"));
				Info.bIsGameStarted = Obj->GetBoolField(TEXT("gameStarted"));

				RoomSummaries.Emplace(Info);
			}
		}
	}
	else
	{
		UE_LOG(LogDD, Log, TEXT("List Rooms Failed: %s"), *Response->GetContentAsString());
	}

	OnListRoomsCompleted.Broadcast(RoomSummaries);
}

void UDDHttpApiSubsystem::OnJoinRoomResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FRoomDetails RoomDetails;

	if ( bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == 200 )
	{
		FString JsonString = Response->GetContentAsString();
		TSharedPtr<FJsonObject> RoomJson;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if ( FJsonSerializer::Deserialize(Reader, RoomJson) && RoomJson.IsValid() )
		{
			RoomDetails.RoomId = RoomJson->GetStringField(TEXT("roomId"));
			RoomDetails.RoomName = RoomJson->GetStringField(TEXT("roomName"));
			RoomDetails.HostUserName = RoomJson->GetStringField(TEXT("hostUsername"));
			RoomDetails.HostIPAddress = RoomJson->GetStringField(TEXT("hostIpAddress"));
			RoomDetails.HostPort = RoomJson->GetIntegerField(TEXT("hostPort"));
			RoomDetails.MaxPlayers = RoomJson->GetIntegerField(TEXT("maxPlayers"));
			RoomDetails.bIsGameStarted = RoomJson->GetBoolField(TEXT("gameStarted"));

			const TArray<TSharedPtr<FJsonValue>>& Arr = RoomJson->GetArrayField(TEXT("players"));
			for ( auto& Player : Arr )
			{
				RoomDetails.Players.Emplace(Player->AsString());
			}

			TSharedPtr<FJsonObject> IPObjs = RoomJson->GetObjectField(TEXT("playerIps"));
			for (auto& IPPair : IPObjs->Values)
			{
				RoomDetails.PlayerIPs.Emplace(IPPair.Key, IPPair.Value->AsString());
			}

			FString Path = FString::Printf(TEXT("%s:%d"), *RoomDetails.HostIPAddress, RoomDetails.HostPort);
			UE_LOG(LogDD, Log, TEXT("Path: %s"), *Path);

			GetWorld()->GetFirstPlayerController()->ClientTravel(Path, TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogDD, Log, TEXT("Join Room Failed"));
	}
}

FString UDDHttpApiSubsystem::GetPort()
{
	FString SystemPort;

	if ( !GConfig->GetString(TEXT("/Script/OnlineSubsystemUtils.IpNetDriver"), TEXT("Port"), SystemPort, GEngineIni) )
	{
		// fallback
		SystemPort = "7777";
	}

	return SystemPort;
}

void UDDHttpApiSubsystem::SetAddress()
{
	bool bCanBind = true;
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
	if (Addr->IsValid())
	{
		IP = Addr->ToString(false);
	}

	Port = GetPort();

	Address = IP + ":" + Port;
	UE_LOG(LogDD, Log, TEXT("SetAddress - %s"), *Address);
}

