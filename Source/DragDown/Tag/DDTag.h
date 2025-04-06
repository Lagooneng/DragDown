#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#define DDTAG_STATE_USINGABILITY	FGameplayTag::RequestGameplayTag(FName("Player.State.UsingAbility"))
#define DDTAG_STATE_DODGE			FGameplayTag::RequestGameplayTag(FName("Player.State.Dodge"))
#define DDTAG_EVENT_DODGEEND		FGameplayTag::RequestGameplayTag(FName("Event.DodgeEnd"))
#define DDTAG_EVENT_PUSHTRIGGER		FGameplayTag::RequestGameplayTag(FName("Event.PushTrigger"))
#define DDTAG_DATA_STAMINAUSED		FGameplayTag::RequestGameplayTag(FName("Data.StaminaUsed"))