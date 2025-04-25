// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DDGASManagerComponent.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/DDPlayerState.h"
#include "Character/DDCharacterBase.h"
#include "DragDown.h"

// Sets default values for this component's properties
UDDGASManagerComponent::UDDGASManagerComponent()
{
	SetIsReplicatedByDefault(true);

	ASC = nullptr;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionPushingRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Pushing.IA_Pushing'"));
	if (nullptr != InputActionPushingRef.Object)
	{
		PushingAction = InputActionPushingRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDodgeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Dodge.IA_Dodge'"));
	if (nullptr != InputActionDodgeRef.Object)
	{
		DodgeAction = InputActionDodgeRef.Object;
	}
}

UAbilitySystemComponent* UDDGASManagerComponent::GetAbilitySystemComponent() const
{
	return ASC;
}

void UDDGASManagerComponent::SetASC()
{
	if (ASC) return;

	AActor* Owner = GetOwner();
	ensure(Cast<APawn>(Owner));

	ADDPlayerState* CSPS = Cast<APawn>(Owner)->GetPlayerState<ADDPlayerState>();
	if (CSPS)
	{
		ASC = CSPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(CSPS, Owner);
		ASC->ReplicationMode = EGameplayEffectReplicationMode::Mixed;
		UE_LOG(LogDD, Log, TEXT("*** [NetMode : %d] SetASC, %s, %s"), GetWorld()->GetNetMode(), *GetName(), *Cast<APawn>(Owner)->GetPlayerState()->GetName());
	}
	else
	{
		UE_LOG(LogDD, Log, TEXT("[NetMode %d] SetASC - ASC Not Found"), GetWorld()->GetNetMode());
	}
}

void UDDGASManagerComponent::SetGASAbilities()
{
	if ( ASC && GetOwner()->HasAuthority() )
	{
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility); 
			ASC->GiveAbility(StartSpec); 
		}

		for (const auto& StartInputAbility : StartInputAbilities) 
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value); 
			StartSpec.InputID = StartInputAbility.Key; 
			ASC->GiveAbility(StartSpec); 
		}
	}
}

void UDDGASManagerComponent::SetupGASInputComponent()
{
	if (IsValid(GetOwner()->InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(GetOwner()->InputComponent);

		//추가 gas 액션
		EnhancedInputComponent->BindAction(PushingAction, ETriggerEvent::Triggered, this, &UDDGASManagerComponent::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(PushingAction, ETriggerEvent::Completed, this, &UDDGASManagerComponent::GASInputReleased, 1);

		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &UDDGASManagerComponent::GASInputPressed, 2);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &UDDGASManagerComponent::GASInputReleased, 2);

		UE_LOG(LogDD, Log, TEXT("SetupGASInputComponent Succeed"));
	}
	else if (!IsValid(ASC))
	{
		UE_LOG(LogDD, Log, TEXT("Invalid ASC"));
	}
	else
	{
		UE_LOG(LogDD, Log, TEXT("Invalid InputComponent"));
	}
}

void UDDGASManagerComponent::GASInputPressed(int32 InputId)
{
	ensure(Cast<ADDCharacterBase>(GetOwner()));
	if ( !Cast<ADDCharacterBase>(GetOwner())->GetActionEnabled() ) return;

	HandleGASInputPressed(InputId);

	if (!GetOwner()->HasAuthority())
	{
		ServerGASInputPressed(InputId);
	}
}

void UDDGASManagerComponent::HandleGASInputPressed(int32 InputId)
{
	if (!ASC)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		if (Spec->InputPressed) return;
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle, true);
		}
	}
}

void UDDGASManagerComponent::GASInputReleased(int32 InputId)
{
	ensure(Cast<ADDCharacterBase>(GetOwner()));
	if (!Cast<ADDCharacterBase>(GetOwner())->GetActionEnabled()) return;

	HandleGASInputReleased(InputId);

	if (!GetOwner()->HasAuthority())
	{
		ServerGASInputReleased(InputId);
	}
}

void UDDGASManagerComponent::HandleGASInputReleased(int32 InputId)
{
	if (!ASC)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void UDDGASManagerComponent::ServerGASInputPressed_Implementation(int32 InputId)
{
	if (GetOwner()->HasAuthority())
	{
		HandleGASInputPressed(InputId);
	}
}

void UDDGASManagerComponent::ServerGASInputReleased_Implementation(int32 InputId)
{
	if ( GetOwner()->HasAuthority() )
	{
		HandleGASInputReleased(InputId);
	}
}




