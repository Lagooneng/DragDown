// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DDInteractionManagerComponent.h"
#include "Character/DDCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interface/DDInteractable.h"
#include "DragDown.h"

// Sets default values for this component's properties
UDDInteractionManagerComponent::UDDInteractionManagerComponent()
{
	SetIsReplicatedByDefault(true);
}


void UDDInteractionManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	ADDCharacterBase* Character = Cast<ADDCharacterBase>(GetOwner());
	if ( Character && Character->GetTrigger() )
	{
		Character->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &UDDInteractionManagerComponent::OnComponentBeginOverlapCallback);
		Character->GetTrigger()->OnComponentEndOverlap.AddDynamic(this, &UDDInteractionManagerComponent::OnComponentEndOverlapCallback);
	}
}

void UDDInteractionManagerComponent::SetInteractionInputComponent()
{
	if ( IsValid( GetOwner()->InputComponent ) )
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(GetOwner()->InputComponent);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UDDInteractionManagerComponent::InteractionInputPressed); 
	}
}

void UDDInteractionManagerComponent::InteractionInputPressed()
{
	if ( GetOwner()->HasAuthority() )
	{
		HandleInteractionInputPressed();
	}
	else
	{
		ServerInteractionInputPressed(); 
	}
}

void UDDInteractionManagerComponent::HandleInteractionInputPressed()
{
	if ( CurrentInteractionActor )
	{
		IDDInteractable* Interaction = Cast<IDDInteractable>(CurrentInteractionActor);
		Interaction->Interact(GetOwner());
	}
}

void UDDInteractionManagerComponent::ServerInteractionInputPressed_Implementation()
{
	HandleInteractionInputPressed(); 
}

void UDDInteractionManagerComponent::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	IDDInteractable* Interaction = Cast<IDDInteractable>(OtherActor);

	if ( Interaction )
	{
		Interaction->BeginInteract(GetOwner());
		CurrentInteractionActor = OtherActor;
	}
}

void UDDInteractionManagerComponent::OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IDDInteractable* Interaction = Cast<IDDInteractable>(OtherActor);

	if (Interaction)
	{
		Interaction->EndInteract(GetOwner());
		CurrentInteractionActor = nullptr;
	}
}
