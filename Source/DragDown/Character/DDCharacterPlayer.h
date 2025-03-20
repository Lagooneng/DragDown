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
class DRAGDOWN_API ADDCharacterPlayer : public ADDCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADDCharacterPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;

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

	// ASC Section
protected:
	void SetupGASInputComponent();

	// Input Pressed RPC **************************************
	void GASInputPressed(int32 InputId);

	UFUNCTION(Server, Reliable)
	void ServerGASInputPressed(int32 InputId);

	void HandleGASInputPressed(int32 InputId);
	// ********************************************************

	// Input Released RPC *************************************
	void GASInputReleased(int32 InputId);

	UFUNCTION(Server, Reliable)
	void ServerGASInputReleased(int32 InputId);

	void HandleGASInputReleased(int32 InputId);
	// ********************************************************

	void SetASC();
	void SetGASAbilities();

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray< TSubclassOf<class UGameplayAbility> > StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap< int32, TSubclassOf<class UGameplayAbility> > StartInputAbilities;
};
