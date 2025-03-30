// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_Gaurd.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "DragDown.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UDDGA_Gaurd::UDDGA_Gaurd()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GaurdMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Montage/AM_Manny_Gaurd.AM_Manny_Gaurd'"));
	if (GaurdMontageRef.Succeeded())
	{
		GaurdMontage = GaurdMontageRef.Object;
	}
}

void UDDGA_Gaurd::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (AvatarCharacter)
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Guard")));
	}

	// Montage task
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		GaurdMontage,     // UAnimMontage* 타입
		1.0f,               // 플레이 속도
		FName(),          // Start Section
		false               // Stop when ability ends
	);

	MontageTask->ReadyForActivation();


}

void UDDGA_Gaurd::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (MontageTask && MontageTask->IsActive())
	{
		MontageTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDDGA_Gaurd::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel)
{
	if (MontageTask && MontageTask->IsActive())
	{
		MontageTask->EndTask();
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancel);
}
