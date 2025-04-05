// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DDBuffManagerComponent.h"
#include "AbilitySystemComponent.h"

// GAS로 예측 실행을 했었는데, 발생 시점을 컨트롤하지 못한다면 예측 실행을 하지 않는게 맞음
// Sets default values for this component's properties
UDDBuffManagerComponent::UDDBuffManagerComponent()
{
	SetIsReplicatedByDefault(true);

	static ConstructorHelpers::FClassFinder<UGameplayEffect> StaminaRegenBuff(TEXT("/Game/Blueprint/GA/GE/BPGE_RegenStamina.BPGE_RegenStamina_C"));
	if(StaminaRegenBuff.Succeeded())
	{
		InitBuffs.Emplace(StaminaRegenBuff.Class);
	}

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
