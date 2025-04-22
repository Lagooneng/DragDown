// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DDGameSettingSubsystem.h"
#include "Setting/DDUserSettings.h"
#include "DragDown.h"

UDDGameSettingSubsystem::UDDGameSettingSubsystem()
{
}

void UDDGameSettingSubsystem::SetShadows(int32 Level)
{
	if (UserSettings == nullptr) return;
	if ( Level > 5 || Level < 0 ) return;

	UserSettings->SetShadowQuality(Level);
	
	ApplyAndSaveAll();
}

void UDDGameSettingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UserSettings = Cast<UDDUserSettings>(GEngine->GetGameUserSettings());

	if ( UserSettings )
	{
		UE_LOG(LogDD, Log, TEXT("UserSettings Found"));
	}
	else
	{
		UE_LOG(LogDD, Error, TEXT("UserSettings not Found"));
	}
}

void UDDGameSettingSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDDGameSettingSubsystem::ApplyAndSaveAll()
{
	if ( UserSettings )
	{
		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}
}