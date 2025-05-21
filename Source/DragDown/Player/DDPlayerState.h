// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "DDPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerInfoChanged);

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADDPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FPlayerInfoChanged OnPlayerInfoChanged;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UDDAttributeSet> AttributeSet;

public:
	FORCEINLINE const FString& GetUserName() { return UserName; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerSetUserName(const FString& InUserName);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_UserName)
	FString UserName;

	UFUNCTION()
	void OnRep_UserName();



};
