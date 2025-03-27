// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/DDTA_MultiTrace.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Physics/DDCollision.h"

ADDTA_MultiTrace::ADDTA_MultiTrace()
{
	bShowDebug = true;
}

void ADDTA_MultiTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ADDTA_MultiTrace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle ADDTA_MultiTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (ASC == nullptr) return FGameplayAbilityTargetDataHandle();

	TArray< FHitResult > OutHitResults;
	const float AttackRange = 50.0f;
	const float AttackRaduis = 100.0f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UCSAT_ReverseGravityTrace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_CSACTION, FCollisionShape::MakeSphere(AttackRaduis), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		for (auto OutHitResult : OutHitResults)
		{
			FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
			DataHandle.Add(TargetData);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsultHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsultHalfHeight, AttackRaduis, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}
#endif

	return DataHandle;
}
