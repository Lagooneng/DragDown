// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DDCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "DDCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDCharacterPlayer : public ADDCharacterBase
{
	GENERATED_BODY()
	
public:
	ADDCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PreInitializeComponents() override;

// Data
protected:
	void SetInputData();
	void SetData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr< class UDDCharacterPlayerData > Data;

	UPROPERTY()
	TObjectPtr<USceneComponent> MeshPivot;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

// GAS Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDDGASManagerComponent> GASManagerComponent;

// Buff
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDDBuffManagerComponent> BuffManagerComponent;

// Surface Detection
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDDSurfaceDetectionComponent> SurfaceDetectionComponent;

protected:
	FTimerHandle TestHandle;
	void Test();
};
