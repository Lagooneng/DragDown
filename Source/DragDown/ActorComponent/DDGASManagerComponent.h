// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "DDGASManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGDOWN_API UDDGASManagerComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	UDDGASManagerComponent();
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void SetASC();

	void SetGASAbilities();

	void SetupGASInputComponent();

protected:

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
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray< TSubclassOf<class UGameplayAbility> > StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap< int32, TSubclassOf<class UGameplayAbility> > StartInputAbilities;

// Action
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PushingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DodgeAction;
};
