// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DDGameSettingSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FSettingsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ViewDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AntiAliasing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PostProcessing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Shadows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GlobalIllumination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Reflections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Textures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Foliage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Shading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsVSyncEnabled;
};

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGameSettingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UDDGameSettingSubsystem();

	UFUNCTION(BlueprintCallable)
	void SetSettings(struct FSettingsStruct InSettings);

	UFUNCTION(BlueprintCallable)
	void ApplyAndSaveAll();

	// View Distance
	UFUNCTION(BlueprintCallable)
	int32 GetViewDistance();

	UFUNCTION(BlueprintCallable)
	void SetViewDistance(int32 Level);

	// Anti Aliasing
	UFUNCTION(BlueprintCallable)
	int32 GetAntiAliasing();

	UFUNCTION(BlueprintCallable)
	void SetAntiAliasing(int32 Level);

	// Post Processing
	UFUNCTION(BlueprintCallable)
	int32 GetPostProcessing();

	UFUNCTION(BlueprintCallable)
	void SetPostProcessing(int32 Level);

	// Shadows
	UFUNCTION(BlueprintCallable)
	int32 GetShadows();

	UFUNCTION(BlueprintCallable)
	void SetShadows(int32 Level);

	// Global Illumination
	UFUNCTION(BlueprintCallable)
	int32 GetGlobalIllumination();

	UFUNCTION(BlueprintCallable)
	void SetGlobalIllumination(int32 Level);

	// Reflection
	UFUNCTION(BlueprintCallable)
	int32 GetReflections();

	UFUNCTION(BlueprintCallable)
	void SetReflections(int32 Level);

	// Texture
	UFUNCTION(BlueprintCallable)
	int32 GetTextures();

	UFUNCTION(BlueprintCallable)
	void SetTextures(int32 Level);

	// Effect
	UFUNCTION(BlueprintCallable)
	int32 GetEffects();

	UFUNCTION(BlueprintCallable)
	void SetEffects(int32 Level);

	// Foliage
	UFUNCTION(BlueprintCallable)
	int32 GetFoliage();

	UFUNCTION(BlueprintCallable)
	void SetFoliage(int32 Level);

	// Shading
	UFUNCTION(BlueprintCallable)
	int32 GetShading();

	UFUNCTION(BlueprintCallable)
	void SetShading(int32 Level);

	// VSync
	UFUNCTION(BlueprintCallable)
	int32 GetVSync();

	UFUNCTION(BlueprintCallable)
	void SetVSync(bool bIsVSync);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	bool CheckGraphicOptionLevel(int32 Level);

	TObjectPtr<class UDDUserSettings> UserSettings;

	int32 GraphicOptionMaxLevel;
	int32 GraphicOptionMinLevel;
};
