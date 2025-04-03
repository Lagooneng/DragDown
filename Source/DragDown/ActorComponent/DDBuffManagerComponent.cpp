// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DDBuffManagerComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
UDDBuffManagerComponent::UDDBuffManagerComponent()
{
	SetIsReplicatedByDefault(true);

	ASC = nullptr;
}

void UDDBuffManagerComponent::Initailize(UAbilitySystemComponent* InASC)
{
	if (ASC != nullptr) return;

	ASC = InASC;

	for (auto Effect : InitBuffs)
	{
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Effect, 1.0f, Context);
		
		if ( EffectSpecHandle.IsValid() )
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}
