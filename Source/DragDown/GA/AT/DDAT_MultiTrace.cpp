// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/DDAT_MultiTrace.h"
#include "GA/TA/DDTA_MultiTrace.h"
#include "AbilitySystemComponent.h"

UDDAT_MultiTrace::UDDAT_MultiTrace()
{
}

UDDAT_MultiTrace* UDDAT_MultiTrace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class ADDTA_MultiTrace> TargetActorClass)
{
	UDDAT_MultiTrace* NewTask = NewAbilityTask<UDDAT_MultiTrace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UDDAT_MultiTrace::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UDDAT_MultiTrace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UDDAT_MultiTrace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ADDTA_MultiTrace>(GetWorld()->SpawnActorDeferred<ADDTA_MultiTrace>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetOwner(GetOwnerActor());
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UDDAT_MultiTrace::OnTargetDataReadyCallback);
	}
}

void UDDAT_MultiTrace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Add(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UDDAT_MultiTrace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
