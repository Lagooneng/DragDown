// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Player/DDPlayerState.h"

ADDPlayerState::ADDPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UDDAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ADDPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
