// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DDGameSettingSubsystem.h"
#include "Setting/DDUserSettings.h"
#include "DragDown.h"

UDDGameSettingSubsystem::UDDGameSettingSubsystem()
{
	GraphicOptionMaxLevel = 5;
	GraphicOptionMinLevel = 0;
}

void UDDGameSettingSubsystem::SetSettings(FSettingsStruct InSettings)
{
	SetViewDistance(InSettings.ViewDistance);
	SetAntiAliasing(InSettings.AntiAliasing);
	SetPostProcessing(InSettings.PostProcessing);
	SetShadows(InSettings.Shadows);
	SetGlobalIllumination(InSettings.GlobalIllumination);
	SetReflections(InSettings.Reflections);
	SetTextures(InSettings.Textures);
	SetEffects(InSettings.Effects);
	SetFoliage(InSettings.Foliage);
	SetShading(InSettings.Shading);
	SetVSync(InSettings.bIsVSyncEnabled);

	ApplyAndSaveAll();
}

int32 UDDGameSettingSubsystem::GetViewDistance()
{
	if (UserSettings)
	{
		return UserSettings->GetViewDistanceQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetViewDistance(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetViewDistanceQuality(Level);
}

int32 UDDGameSettingSubsystem::GetAntiAliasing()
{
	if ( UserSettings )
	{
		UserSettings->GetAntiAliasingQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetAntiAliasing(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetAntiAliasingQuality(Level);
}

int32 UDDGameSettingSubsystem::GetPostProcessing()
{
	if (UserSettings)
	{
		UserSettings->GetPostProcessingQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetPostProcessing(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetPostProcessingQuality(Level);
}

int32 UDDGameSettingSubsystem::GetShadows()
{
	if (UserSettings)
	{
		UserSettings->GetShadowQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetShadows(int32 Level)
{
	if (UserSettings == nullptr) return;
	if ( !CheckGraphicOptionLevel(Level) ) return;

	UserSettings->SetShadowQuality(Level);
}

int32 UDDGameSettingSubsystem::GetGlobalIllumination()
{
	if (UserSettings)
	{
		UserSettings->GetGlobalIlluminationQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetGlobalIllumination(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetGlobalIlluminationQuality(Level);
}

int32 UDDGameSettingSubsystem::GetReflections()
{
	if (UserSettings)
	{
		UserSettings->GetReflectionQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetReflections(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetReflectionQuality(Level);
}

int32 UDDGameSettingSubsystem::GetTextures()
{
	if (UserSettings)
	{
		UserSettings->GetTextureQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetTextures(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetTextureQuality(Level);
}

int32 UDDGameSettingSubsystem::GetEffects()
{
	if (UserSettings)
	{
		UserSettings->GetVisualEffectQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetEffects(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetVisualEffectQuality(Level);
}

int32 UDDGameSettingSubsystem::GetFoliage()
{
	if (UserSettings)
	{
		UserSettings->GetFoliageQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetFoliage(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetFoliageQuality(Level);
}

int32 UDDGameSettingSubsystem::GetShading()
{
	if (UserSettings)
	{
		UserSettings->GetShadingQuality();
	}
	return 0;
}

void UDDGameSettingSubsystem::SetShading(int32 Level)
{
	if (UserSettings == nullptr) return;
	if (!CheckGraphicOptionLevel(Level)) return;

	UserSettings->SetShadingQuality(Level);
}

int32 UDDGameSettingSubsystem::GetVSync()
{
	if ( UserSettings )
	{
		return UserSettings->IsVSyncEnabled();
	}
	return false;
}

void UDDGameSettingSubsystem::SetVSync(bool bIsVSync)
{
	if (UserSettings == nullptr) return;

	UserSettings->SetVSyncEnabled(bIsVSync);
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

bool UDDGameSettingSubsystem::CheckGraphicOptionLevel(int32 Level)
{
	return (Level <= GraphicOptionMaxLevel && Level >= GraphicOptionMinLevel);
}

void UDDGameSettingSubsystem::ApplyAndSaveAll()
{
	if ( UserSettings )
	{
		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}
}


