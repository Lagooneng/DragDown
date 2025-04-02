// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDAttackStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGDOWN_API UDDAttackStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDDAttackStateComponent();
	
	FORCEINLINE int8 GetAttackState() { return NowAttackState; }
	FORCEINLINE void PlusAttackState() { 
		NowAttackState = (NowAttackState + 1) % MaxAttackState;
		UE_LOG(LogTemp, Log, TEXT("PlusAttackState - %d, %d"), NowAttackState, MaxAttackState);
	}

	FString GetSectionPrefix();
	float GetPower();
	float GetZPower();
	float GetNecessaryStamina();

private:
	UPROPERTY()
	TObjectPtr < class UDDStateDrivenAttackData > StateDrivenAttackData;

	int8 NowAttackState;
	int8 MaxAttackState;
};
