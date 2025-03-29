// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_PushingCharacter.h"
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
#include "ActorComponent/DDAttackStateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UDDGA_PushingCharacter::UDDGA_PushingCharacter()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted; 
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PushingMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Montage/AM_Manny_Pushing.AM_Manny_Pushing'"));
	if ( PushingMontageRef.Succeeded() )
	{
		PushingMontage = PushingMontageRef.Object;
	}
}

void UDDGA_PushingCharacter::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if ( AvatarCharacter )
	{
		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AttackStateComponent = AvatarCharacter->GetComponentByClass<UDDAttackStateComponent>();
	}

	if (AttackStateComponent == nullptr) return;

	FString SectionString = AttackStateComponent->GetSectionPrefix() + FString::FromInt(AttackStateComponent->GetAttackState());
	FName SectionName(*SectionString);

	// Montage task
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		PushingMontage,     // UAnimMontage* 타입
		1.0f,               // 플레이 속도
		SectionName,          // Start Section
		false               // Stop when ability ends
	);

	MontageTask->OnCompleted.AddDynamic(this, &UDDGA_PushingCharacter::OnMontageCompleted);
	MontageTask->ReadyForActivation();

	// Wait task
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.PushTrigger"))
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_PushingCharacter::OnPushingEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_PushingCharacter::OnMontageCompleted()
{
	AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UDDGA_PushingCharacter::OnPushingEventReceived(FGameplayEventData Payload)
{
	UDDAT_MultiTrace* TraceTask = UDDAT_MultiTrace::CreateTask(this, ADDTA_MultiTrace::StaticClass());
	TraceTask->OnComplete.AddDynamic(this, &UDDGA_PushingCharacter::OnTraceResultCallback);
	TraceTask->ReadyForActivation();
}

void UDDGA_PushingCharacter::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	UE_LOG(LogDD, Log, TEXT("OnTraceResultCallback"));

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
		ProcessPush(TargetDataHandle);
	}
	else
	{
		ProcessPush(TargetDataHandle);
	}

	// 클라이언트: 서버 검증 요청
	if (!AvatarCharacter->HasAuthority())
	{
		Server_ValidatePush(TargetDataHandle);
	}

	AttackStateComponent->PlusAttackState(); 
}

void UDDGA_PushingCharacter::ProcessPush(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	int32 Idx = 0;
	while (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, Idx))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, Idx);

		ACharacter* Character = Cast<ACharacter>(HitResult.GetActor());
		if (Character)
		{
			if (AttackStateComponent == nullptr) return;

			FVector LaunchDirection = AvatarCharacter->GetController()->GetControlRotation().Vector(); // 내가 바라보는 방향

			FVector LaunchVelocity = LaunchDirection * AttackStateComponent->GetPower();
			LaunchVelocity.Z += AttackStateComponent->GetZPower();

			Character->LaunchCharacter(LaunchVelocity, true, true);
		}

		UE_LOG(LogDD, Log, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());

		++Idx;
	}
}

void UDDGA_PushingCharacter::Server_ValidatePush_Implementation(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		FScopedPredictionWindow ScopedPrediction(ASC, false);
		ProcessPush(TargetDataHandle);
	}
	else
	{
		ProcessPush(TargetDataHandle);
	}
}

bool UDDGA_PushingCharacter::Server_ValidatePush_Validate(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 필요한 검증 조건 구현 (스테미나 검증 등)
	return true;
}