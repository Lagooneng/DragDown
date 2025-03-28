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
	
	if (ActivationInfo.GetActivationPredictionKey().IsValidKey())
	{
		FDateTime Now = FDateTime::Now();
		FString Timestamp = FString::Printf(TEXT("%d-%02d-%02d %02d:%02d:%02d.%03d"),
			Now.GetYear(), Now.GetMonth(), Now.GetDay(),
			Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());

		UE_LOG(LogDD, Warning, TEXT("[%s][NetMode %d] Client-side(%s) prediction running"),
			*Timestamp, GetWorld()->GetNetMode(), *ActorInfo->AvatarActor.Get()->GetName());
	}

	if (HasAuthority(&ActivationInfo))
	{
		FDateTime Now = FDateTime::Now();
		FString Timestamp = FString::Printf(TEXT("%d-%02d-%02d %02d:%02d:%02d.%03d"),
			Now.GetYear(), Now.GetMonth(), Now.GetDay(),
			Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());

		UE_LOG(LogDD, Warning, TEXT("[%s][NetMode %d] Server-side(%s) authority running"),
			*Timestamp, GetWorld()->GetNetMode(), *ActorInfo->AvatarActor.Get()->GetName());
	}

	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	// Montage task
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		PushingMontage,     // UAnimMontage* 타입
		1.0f,               // 플레이 속도
		FName(TEXT("ComboAttack1")),          // Start Section
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

	// Target & Damage

}

void UDDGA_PushingCharacter::OnMontageCompleted()
{
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

	int32 Idx = 0;
	while ( UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, Idx) )
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, Idx);

		ACharacter* Character = Cast<ACharacter>(HitResult.GetActor());
		if (Character)
		{
			FVector LaunchDirection = AvatarCharacter->GetController()->GetControlRotation().Vector(); // 내가 바라보는 방향
			float LaunchStrength = 1000.0f; // 밀어내는 세기 (원하는 대로 조정)

			FVector LaunchVelocity = LaunchDirection * LaunchStrength;
			Character->LaunchCharacter(LaunchVelocity, true, true);
		}

		UE_LOG(LogDD, Log, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());

		++Idx;
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
