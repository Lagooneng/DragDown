// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADDPlayerController();

protected:
	virtual void BeginPlayingState() override;

protected:
	void InitGASWidget();

	// UMG 위젯 클래스 (블루프린트에서 설정)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UDDGASStaminaBarUserWidget> StaminaBarWidgetClass;

	UPROPERTY()
	TObjectPtr<class UDDGASStaminaBarUserWidget> StaminaBarWidget;
};
