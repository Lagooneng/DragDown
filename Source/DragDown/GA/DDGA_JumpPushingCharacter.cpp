// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_JumpPushingCharacter.h"
#include "DDGA_JumpPushingCharacter.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GA/AT/DDAT_MultiTrace.h"
#include "GA/TA/DDTA_MultiTrace.h"
#include "Misc/DateTime.h"
#include "DragDown.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UDDGA_JumpPushingCharacter::UDDGA_JumpPushingCharacter()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PushingMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Montage/AM_Manny_JumpPushingCharacter.AM_Manny_JumpPushingCharacter'"));
	if (PushingMontageRef.Succeeded())
	{
		PushingMontage = PushingMontageRef.Object;
	}

	bIsTraced = false;
}

void UDDGA_JumpPushingCharacter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bIsTraced = false;

	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	/*if (AvatarCharacter)
	{
		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}*/


	// Montage task
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		PushingMontage,     // UAnimMontage* 타입
		1.0f,               // 플레이 속도
		FName("Start"),          // Start Section
		false               // Stop when ability ends
	);

	MontageTask->OnCompleted.AddDynamic(this, &UDDGA_JumpPushingCharacter::OnMontageCompleted);
	MontageTask->ReadyForActivation();

	if (AvatarCharacter) 
	{
		AvatarCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 500.0f), false, true); 
	}

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.PushTrigger"))
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_JumpPushingCharacter::OnPushingEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_JumpPushingCharacter::OnMontageCompleted()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UDDGA_JumpPushingCharacter::OnPushingEventReceived(FGameplayEventData Payload)
{
	UDDAT_MultiTrace* TraceTask = UDDAT_MultiTrace::CreateTask(this, ADDTA_MultiTrace::StaticClass());
	TraceTask->OnComplete.AddDynamic(this, &UDDGA_JumpPushingCharacter::OnTraceResultCallback);
	TraceTask->ReadyForActivation();
}

void UDDGA_JumpPushingCharacter::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (bIsTraced) return;
	bIsTraced = true;
	UE_LOG(LogDD, Log, TEXT("OnTraceResultCallback"));

	if ( AvatarCharacter )
	{
		AvatarCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 300.0f), false, true);
	}

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
		ProcessPush(TargetDataHandle);
	}
}

void UDDGA_JumpPushingCharacter::ProcessPush(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	int32 Idx = 0;
	while (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, Idx))
	{
		ACharacter* Character = nullptr;

		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, Idx);
		if (HitResult.GetActor())
		{
			Character = Cast<ACharacter>(HitResult.GetActor());
		}

		if (Character)
		{
			if (AvatarCharacter == nullptr)
			{
				UE_LOG(LogDD, Log, TEXT("UDDGA_PushingCharacter::ProcessPush - No AvatarCharacter"));
				return;
			}

			FVector LaunchDirection = AvatarCharacter->GetController()->GetControlRotation().Vector(); // 내가 바라보는 방향
			float Power = 2000.0f;
			float ZPower = 500.0f;
			FVector LaunchVelocity = LaunchDirection * Power;
			LaunchVelocity.Z = 0.0f;
			LaunchVelocity.Z += ZPower;

			Character->LaunchCharacter(LaunchVelocity, true, true);
		}

		UE_LOG(LogDD, Log, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());

		++Idx;
	}
}

void UDDGA_JumpPushingCharacter::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	bIsTraced = false;

	if (AvatarCharacter)
	{
		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}
