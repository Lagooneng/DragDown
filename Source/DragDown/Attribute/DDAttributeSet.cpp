// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/DDAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "DragDown.h"

UDDAttributeSet::UDDAttributeSet() : MaxStamina(100.0f), Damage(0.0f)
{
	InitStamina(GetMaxStamina());
}

void UDDAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

void UDDAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumStamina = 0.0f;

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), MinimumStamina, GetMaxStamina()));
		UE_LOG(LogDD, Warning, TEXT("Direct Health Access : %f"), GetStamina());
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina() - GetDamage(), MinimumStamina, GetMaxStamina()));
		UE_LOG(LogDD, Log, TEXT("[NetMode : %d] Damage Detected : %f | Now Energy : %f"), GetWorld()->GetNetMode(), GetDamage(), GetStamina());
	}
}

void UDDAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDDAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UDDAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDAttributeSet, Stamina, OldStamina);
}

void UDDAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDAttributeSet, MaxStamina, OldMaxStamina); 
}
