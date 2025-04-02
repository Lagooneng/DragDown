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
#include "Character/DDCharacterBase.h"

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
	Power = 400.0f;
	ZPower = 800.0f;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UsingAbility")));
}

void UDDGA_JumpPushingCharacter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bIsTraced = false;

	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UsingAbility")));
	}

	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	// UAbilityTask_PlayMontageAndWait를 썼었는데 플레이어가 Multicast 날리는 거로 변경
	// 이유: 클라이언트A가 클라이언트 B, C의 애니메이션을 UAbilityTask_PlayMontageAndWait로 복제받으면 느림
	// RPC가 훨씬 빨라서 다른 클라이언트 애니메이션 동기화가 더 잘되고
	// 본인은 Local Prediction을 통해 지연 완화
	ADDCharacterBase* Character = Cast<ADDCharacterBase>(AvatarCharacter);
	if (Character)
	{
		UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
		if (ASC)
		{
			FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
			Character->NetMulticastPlayAnimMontage(PushingMontage, FName("Start"));
		}
	}

	if ( AvatarCharacter ) 
	{
		AvatarCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 300.0f), true, true);
	}

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.PushTrigger"))
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_JumpPushingCharacter::OnPushingEventReceived);
	EventTask->ReadyForActivation();
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
		AvatarCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 300.0f), true, true);
	}

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
		ProcessPush(TargetDataHandle);
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
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
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);

			if (TargetASC && TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Dodge"))))
			{
				UE_LOG(LogDD, Log, TEXT("Character %s is dodging - skipping LaunchCharacter"), *Character->GetName());
				++Idx;
				continue;
			}

			if (AvatarCharacter == nullptr)
			{
				UE_LOG(LogDD, Log, TEXT("UDDGA_PushingCharacter::ProcessPush - No AvatarCharacter"));
				return;
			}

			FVector LaunchDirection = AvatarCharacter->GetController()->GetControlRotation().Vector(); // 내가 바라보는 방향
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
	bIsTraced = false;

	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.UsingAbility")));
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDDGA_JumpPushingCharacter::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel)
{
	if (EventTask && EventTask->IsActive())
	{
		EventTask->EndTask();
	}

	// for Local Prediction Role Back
	bIsTraced = false;

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancel);
}