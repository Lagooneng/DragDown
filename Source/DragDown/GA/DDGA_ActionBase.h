// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DDGA_ActionBase.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UDDGA_ActionBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDDGA_ActionBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void PlayAnim(FName SectionName);

	UFUNCTION()
	virtual void OnAnimEnd(FGameplayEventData Payload);

	virtual void SetData();

	void EnableInput(); 
	void DisableInput();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<class UDDActionAbilityData> AbilityData; 

	UPROPERTY()
	TObjectPtr<UAnimMontage> ActionMontage;

	UPROPERTY()
	TSubclassOf< class UGameplayEffect > DownStaminaEffect;

	UPROPERTY()
	TObjectPtr< class UAbilityTask_PlayMontageAndWait > MontageTask;

	UPROPERTY()
	TObjectPtr < class UAbilityTask_WaitGameplayEvent > EventTask;

	UPROPERTY()
	TObjectPtr < class UAbilityTask_WaitGameplayEvent > AnimEndTask;

	UPROPERTY()
	TObjectPtr<class ADDCharacterBase> AvatarCharacter;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	bool bIsEventTriggered;
};
