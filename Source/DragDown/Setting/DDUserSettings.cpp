// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting/DDUserSettings.h"

UDDUserSettings::UDDUserSettings()
{
	BGMVolume = 1.0f;
	SFXVolume = 1.0f;

	LoadConfig();
}

void UDDUserSettings::ApplySettings(bool bForce)
{
	Super::ApplySettings(bForce);

	SetBGMVolume(BGMVolume);
	SetSFXVolume(SFXVolume);
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
