// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/DDMeshDatas.h"
#include "DDCharacterChangeButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDCharacterChangeButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetMeshID(EMESHID InMeshID) { MeshID = InMeshID; }

	UFUNCTION(BlueprintCallable)
	void SetMeshDescription(FText InDescription);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnChangeCharacterAppearance;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtChangeCharacterAppearance;

	EMESHID MeshID;

	UFUNCTION()
	void OnClickedCallback();

	bool CanChangeAppearance();
};
