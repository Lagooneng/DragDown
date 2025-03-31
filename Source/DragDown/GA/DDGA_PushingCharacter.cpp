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
#include "GA/DDGA_JumpPushingCharacter.h"
#include "Character/DDCharacterBase.h"

UDDGA_PushingCharacter::UDDGA_PushingCharacter()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted; 
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PushingMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Montage/AM_Manny_Pushing.AM_Manny_Pushing'"));
	if ( PushingMontageRef.Succeeded() )
	{
		PushingMontage = PushingMontageRef.Object;
	}

	bIsTraced = false;
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

	bIsTraced = false;

	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if ( AvatarCharacter )
	{
		if (AvatarCharacter->GetCharacterMovement()->IsFalling())
		{
			if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
			{
				ASC->TryActivateAbilityByClass(UDDGA_JumpPushingCharacter::StaticClass());
			}
			
			bool bReplicatedEndAbility = true;
			bool bWasCancelled = false;
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AttackStateComponent = AvatarCharacter->GetComponentByClass<UDDAttackStateComponent>();
	}

	if (AttackStateComponent == nullptr) return;

	FString SectionString = AttackStateComponent->GetSectionPrefix() + FString::FromInt(AttackStateComponent->GetAttackState());
	FName SectionName(*SectionString);

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
			Character->NetMulticastPlayAnimMontage(PushingMontage, SectionName);
		}
	}

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.PushTrigger"))
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_PushingCharacter::OnPushingEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_PushingCharacter::OnPushingEventReceived(FGameplayEventData Payload)
{
	UDDAT_MultiTrace* TraceTask = UDDAT_MultiTrace::CreateTask(this, ADDTA_MultiTrace::StaticClass());
	TraceTask->OnComplete.AddDynamic(this, &UDDGA_PushingCharacter::OnTraceResultCallback);
	TraceTask->ReadyForActivation();
}

void UDDGA_PushingCharacter::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (bIsTraced) return;
	bIsTraced = true;
	UE_LOG(LogDD, Log, TEXT("[NetMode : %d] OnTraceResultCallback"), GetWorld()->GetNetMode());

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
		ProcessPush(TargetDataHandle);
		AttackStateComponent->PlusAttackState();
	}

	bool bReplicatedEndAbility = true; 
	bool bWasCancelled = false; 
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled); 
}

void UDDGA_PushingCharacter::ProcessPush(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
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
			if ( AttackStateComponent == nullptr )
			{
				UE_LOG(LogDD, Log, TEXT("UDDGA_PushingCharacter::ProcessPush - No AttackStateComponent"));
				return;
			}

			if ( AvatarCharacter == nullptr )
			{
				UE_LOG(LogDD, Log, TEXT("UDDGA_PushingCharacter::ProcessPush - No AvatarCharacter"));
				return;
			}

			FVector LaunchDirection = AvatarCharacter->GetController()->GetControlRotation().Vector(); // 내가 바라보는 방향

			FVector LaunchVelocity = LaunchDirection * AttackStateComponent->GetPower();
			LaunchVelocity.Z = 0.0f;
			LaunchVelocity.Z += AttackStateComponent->GetZPower();

			Character->LaunchCharacter(LaunchVelocity, true, true);
		}

		UE_LOG(LogDD, Log, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());

		++Idx;
	}
}

void UDDGA_PushingCharacter::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	bIsTraced = false;

	if ( AvatarCharacter )
	{
		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDDGA_PushingCharacter::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel)
{
	if ( EventTask && EventTask->IsActive() )
	{
		EventTask->EndTask();
	}

	// for Local Prediction Role Back
	if (AvatarCharacter)
	{
		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	bIsTraced = false;

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancel);
}
