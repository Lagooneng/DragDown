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

	virtual void BeginPlay() override;
	
	FORCEINLINE int8 GetAttackState() { return NowAttackState; }

	void PlusAttackState();

	FString GetSectionPrefix();
	float GetPower();
	float GetZPower();
	float GetNecessaryStamina();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr < class UDDStateDrivenAttackData > StateDrivenAttackData;

	UPROPERTY(Replicated)
	int8 NowAttackState;

	int8 MaxAttackState;
};
