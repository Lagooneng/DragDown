// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDPortalActorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/DDPromptWidget.h"
#include "Physics/DDCollision.h"
#include "DragDown.h"

ADDPortalActorBase::ADDPortalActorBase()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetIsReplicated(true);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComp;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetSphereRadius(80.0f, true);
	Trigger->SetCollisionProfileName( CPROFILE_OVERLAPALL );
	Trigger->SetupAttachment(StaticMeshComp);

	InteractionPromptComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptComponent"));
	InteractionPromptComponent->SetupAttachment(StaticMeshComp);
	InteractionPromptComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	InteractionPromptComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionPromptComponent->SetDrawSize(FVector2D(500.0f, 30.0f));
	InteractionPromptComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionPromptComponent->SetVisibility(false);
}

void ADDPortalActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	UUserWidget* UserWidget = InteractionPromptComponent->GetWidget(); 

	if ( UserWidget ) 
	{
		PromptWidget = Cast<UDDPromptWidget>(UserWidget);
	}
}

void ADDPortalActorBase::Interact(AActor* InterActor)
{
	if (!CanInteract(InterActor)) return;
}

bool ADDPortalActorBase::CanInteract(AActor* InterActor)
{
	return true;
}

void ADDPortalActorBase::BeginInteract(AActor* InterActor)
{
	APawn* Pawn = Cast<APawn>(InterActor);
	if ( Pawn && !Pawn->IsLocallyControlled() )
	{
		return;
	}

	if ( InteractionPromptComponent && PromptWidget )
	{
		PromptWidget->SetDescription( GetInteractionText(InterActor) );
		InteractionPromptComponent->SetVisibility(true);
	}
	else
	{
		UE_LOG(LogDD, Error, TEXT("BeginInteract is failed"));
	}
}

void ADDPortalActorBase::EndInteract(AActor* InterActor)
{
	APawn* Pawn = Cast<APawn>(InterActor);
	if (Pawn && !Pawn->IsLocallyControlled())
	{
		return;
	}

	if ( InteractionPromptComponent )
	{
		InteractionPromptComponent->SetVisibility(false);
	}
}

FText ADDPortalActorBase::GetInteractionText(AActor* InterActor)
{
	return DefaultDescription;
}

