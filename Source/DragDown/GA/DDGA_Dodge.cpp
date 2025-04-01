// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/DDGA_Dodge.h"
#include "DDGA_Dodge.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "DragDown.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/DDCharacterBase.h"
#include "Misc/DateTime.h"

UDDGA_Dodge::UDDGA_Dodge()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Montage/AM_Manny_Dodge.AM_Manny_Dodge'"));
	if (DodgeMontageRef.Succeeded())
	{
		DodgeMontage = DodgeMontageRef.Object;
	}

	bIsDodged = false;
}

void UDDGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	bIsDodged = false;

	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (AvatarCharacter)
	{
		if (AvatarCharacter->GetCharacterMovement()->IsFalling())
		{
			bool bReplicatedEndAbility = true;
			bool bWasCancelled = false;
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}

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
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Dodge")));

			FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
			Character->NetMulticastPlayAnimMontage(DodgeMontage, FName());
		}
	}

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.DodgeEnd"))
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_Dodge::OnDodgeEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_Dodge::OnDodgeEventReceived(FGameplayEventData Payload)
{
	UE_LOG(LogDD, Log, TEXT("[NetMode : %d] OnDodgeEventReceived"), GetWorld()->GetNetMode());

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Dodge")));
	}

	bool bReplicatedEndAbility = true; 
	bool bWasCancelled = false; 
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled); 
}

void UDDGA_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (EventTask && EventTask->IsActive())
	{
		EventTask->EndTask();
	}

	// for Local Prediction Role Back
	if (AvatarCharacter)
	{
		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); 
	}

	bIsDodged = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled); 
}

void UDDGA_Dodge::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancel)
{
	if (EventTask && EventTask->IsActive())
	{
		EventTask->EndTask();
	}

	// for Local Prediction Role Back
	if (AvatarCharacter)
	{
		AvatarCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	bIsDodged = false;

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancel);
}
