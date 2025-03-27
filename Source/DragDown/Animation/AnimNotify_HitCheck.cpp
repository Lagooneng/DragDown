// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_HitCheck.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Actor.h"
#include "Character/DDCharacterPlayer.h"

void UAnimNotify_HitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;


	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC) return;

	FGameplayEventData EventData;
	FGameplayTag HitTag = FGameplayTag::RequestGameplayTag(FName("Event.PushTrigger"));
	EventData.EventTag = HitTag;
	EventData.Instigator = Owner;
	EventData.Target = Owner;

	ASC->HandleGameplayEvent(HitTag, &EventData);
}
