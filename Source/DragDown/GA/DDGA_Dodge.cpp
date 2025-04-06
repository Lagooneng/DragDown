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
#include "Attribute/DDAttributeSet.h"
#include "Tag/DDTag.h"

UDDGA_Dodge::UDDGA_Dodge()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Montage/AM_Manny_Dodge.AM_Manny_Dodge'"));
	if (DodgeMontageRef.Succeeded())
	{
		ActionMontage = DodgeMontageRef.Object;
	}

	NecessaryStamina = 10.0f;
}

void UDDGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bIsAvtivated = false;

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
	if (AvatarCharacter)
	{
		if (ASC)
		{
			ASC->AddLooseGameplayTag(DDTAG_STATE_DODGE);
			ASC->AddLooseGameplayTag(DDTAG_STATE_USINGABILITY);

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DownStaminaEffect);
			if (EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(DDTAG_DATA_STAMINAUSED, -NecessaryStamina);
				ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
			}

			FScopedPredictionWindow ScopedPrediction(ASC, !AvatarCharacter->HasAuthority());
			AvatarCharacter->NetMulticastPlayAnimMontage(ActionMontage, FName());
		}
	}

	// Wait task
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		DDTAG_EVENT_DODGEEND
	);

	EventTask->EventReceived.AddDynamic(this, &UDDGA_Dodge::OnDodgeEventReceived);
	EventTask->ReadyForActivation();
}

void UDDGA_Dodge::OnDodgeEventReceived(FGameplayEventData Payload)
{
	UE_LOG(LogDD, Log, TEXT("[NetMode : %d] OnDodgeEventReceived"), GetWorld()->GetNetMode());

	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(DDTAG_STATE_DODGE);
	}

	bool bReplicatedEndAbility = true; 
	bool bWasCancelled = false; 
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled); 
}

bool UDDGA_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (ASC == nullptr) return false;

	const UDDAttributeSet* AttributeSet = ASC->GetSet<UDDAttributeSet>();
	if (AttributeSet == nullptr) return false;

	float CurrentStamina = AttributeSet->GetStamina();

	if (CurrentStamina < NecessaryStamina)
	{
		return false;
	}

	return true;
}
