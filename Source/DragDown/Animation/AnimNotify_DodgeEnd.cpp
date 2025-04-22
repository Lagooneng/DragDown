// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_DodgeEnd.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Actor.h"
#include "Tag/DDTag.h"

void UAnimNotify_DodgeEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC) return;

	FGameplayEventData EventData;
	FGameplayTag DodgeEndTag = DDTAG_EVENT_DODGEEND;
	EventData.EventTag = DodgeEndTag;
	EventData.Instigator = Owner;
	EventData.Target = Owner;

	ASC->HandleGameplayEvent(DodgeEndTag, &EventData);
}
