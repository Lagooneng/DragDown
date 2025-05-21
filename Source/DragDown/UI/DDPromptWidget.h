// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDPromptWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDPromptWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetDescription( const FText& InDescription );

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Description;

};
