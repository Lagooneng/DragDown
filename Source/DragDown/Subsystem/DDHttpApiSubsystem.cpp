// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DDHttpApiSubsystem.h"
#include "HttpModule.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "DragDown.h"

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
	Request->OnProcessRequestComplete().BindUObject(this, &UDDHttpApiSubsystem::OnResponseReceived);
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
	Request->OnProcessRequestComplete().BindUObject(this, &UDDHttpApiSubsystem::OnResponseReceived);
	Request->ProcessRequest();
}

void UDDHttpApiSubsystem::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		int32 ResponseCode = Response->GetResponseCode();
		FString ResponseContent = Response->GetContentAsString();

		UE_LOG(LogDD, Log, TEXT("ResponseCode: %d"), ResponseCode);
		UE_LOG(LogDD, Log, TEXT("ResponseContent: %s"), *ResponseContent);
	}
	else
	{
		UE_LOG(LogDD, Error, TEXT("Request Failed"));
	}
}

