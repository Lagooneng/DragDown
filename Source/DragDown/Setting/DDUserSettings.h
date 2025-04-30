// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DLSSLibrary.h"
#include "DDUserSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DRAGDOWN_API UDDUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UDDUserSettings();

	virtual void ApplySettings(bool bForce = false) override;

// Sound
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMasterVolume() { return MasterVolume; }

	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float InMasterVolume);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetBGMVolume() { return BGMVolume; }

	UFUNCTION(BlueprintCallable)
	void SetBGMVolume(float InBGMVolume);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetSFXVolume() { return SFXVolume; }

	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float InSFXVolume);

// DLSS
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDLSSMode& GetDLSSMode() { return DLSSMode; }

	UFUNCTION(BlueprintCallable)
	void SetDLSSMode(UDLSSMode InDLSSMode);

	UFUNCTION(BlueprintCallable)
	void ApplyDLSSMode();

	UFUNCTION(BlueprintCallable)
	bool CheckDLSSAvailable();

protected:
	UPROPERTY(Config)
	float MasterVolume;

	UPROPERTY(Config)
	float BGMVolume;

	UPROPERTY(Config)
	float SFXVolume;

	UPROPERTY(Config)
	UDLSSMode DLSSMode;
};
