// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DDCharacterBase.h"
#include "Interface/DDPlayerFinderInterface.h"
#include "AbilitySystemInterface.h"
#include "DDCharacterPatrol.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API ADDCharacterPatrol : public ADDCharacterBase, public IDDPlayerFinderInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADDCharacterPatrol();

	virtual void BeginPlay() override;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual class ADDCharacterPlayer* GetCharacterPlayer() override;

	FVector GetPatrolPos();

	void PushCharacter();

protected:
	UPROPERTY()
	TObjectPtr<class ADDCharacterPlayer> Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPos")
	TArray< FVector > PatrolPoses;

	int8 CurrentPatrolIdx;
	int8 PatrolPosesLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Power;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

protected:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> PlayerSensor;

	UPROPERTY()
	TObjectPtr<class ADDCharacterPlayer> CharacterPlayer;
};
