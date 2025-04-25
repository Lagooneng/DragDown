// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DDUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UDDUserSettings();

	virtual void ApplySettings(bool bForce = false) override;

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

protected:
	UPROPERTY(Config)
	float MasterVolume;

	UPROPERTY(Config)
	float BGMVolume;

	UPROPERTY(Config)
	float SFXVolume;
};
