// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting/DDUserSettings.h"
#include "DLSSLibrary.h"

UDDUserSettings::UDDUserSettings()
{
	MasterVolume = 1.0f;
	BGMVolume = 1.0f;
	SFXVolume = 1.0f;
	DLSSMode = UDLSSMode::Off;
	LoadConfig();
}

void UDDUserSettings::ApplySettings(bool bForce)
{
	Super::ApplySettings(bForce);
	
	SetMasterVolume(MasterVolume);
	SetBGMVolume(BGMVolume);
	SetSFXVolume(SFXVolume);
	SetDLSSMode(DLSSMode);

	ApplyDLSSMode();

	SaveSettings();
}

void UDDUserSettings::SetMasterVolume(float InMasterVolume)
{
	MasterVolume = FMath::Clamp(InMasterVolume, 0.0f, 1.0f);
	SaveConfig();
}

void UDDUserSettings::SetBGMVolume(float InBGMVolume)
{
	BGMVolume = FMath::Clamp(InBGMVolume, 0.0f, 1.0f);
	SaveConfig();
}

void UDDUserSettings::SetSFXVolume(float InSFXVolume)
{
	SFXVolume = FMath::Clamp(InSFXVolume, 0.0f, 1.0f);
	SaveConfig();
}

void UDDUserSettings::SetDLSSMode(UDLSSMode InDLSSMode)
{
	DLSSMode = InDLSSMode;
	SaveConfig();
}

void UDDUserSettings::ApplyDLSSMode()
{
	if (!CheckDLSSAvailable()) return;

	UDLSSLibrary::SetDLSSMode(GetWorld(), DLSSMode);
}

bool UDDUserSettings::CheckDLSSAvailable()
{
	return UDLSSLibrary::IsDLSSSupported();
}
