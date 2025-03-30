// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DDCharacterBase.generated.h"

UCLASS()
class DRAGDOWN_API ADDCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADDCharacterBase();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastPlayAnimMontage(UAnimMontage* Montage, FName SectionName);

protected:
	TObjectPtr<class UDDAttackStateComponent> AttackStateComponent;
};
