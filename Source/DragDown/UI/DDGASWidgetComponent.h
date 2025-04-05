// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DDGASWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGASWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void ActivateGAS(); 

protected:
	virtual void InitWidget() override; 
};
