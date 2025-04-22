// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DDGameSettingSubsystem.generated.h"

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
	void SetShadows(int32 Level);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void ApplyAndSaveAll();


	TObjectPtr<class UDDUserSettings> UserSettings;
};
