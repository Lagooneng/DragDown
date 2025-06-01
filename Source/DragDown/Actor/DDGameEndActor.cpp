// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDGameEndActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/DDPromptWidget.h"
#include "Physics/DDCollision.h"
#include "Game/DDPlayingGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/DDPlayerController.h"
#include "DragDown.h"

// Sets default values
ADDGameEndActor::ADDGameEndActor()
{
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetIsReplicated(true);
	RootComponent = StaticMeshComp;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetSphereRadius(80.0f, true);
	Trigger->SetCollisionProfileName(CPROFILE_OVERLAPALL);
	Trigger->SetupAttachment(StaticMeshComp);

	InteractionPromptComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptComponent"));
	InteractionPromptComponent->SetupAttachment(StaticMeshComp);
	InteractionPromptComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	InteractionPromptComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionPromptComponent->SetDrawSize(FVector2D(500.0f, 30.0f));
	InteractionPromptComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionPromptComponent->SetVisibility(false);

	FadeDuration = 5.0f;
	bIsInteracted = false;
	Launch = FVector(0.0f, 0.0f, 0.0f);
}

void ADDGameEndActor::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* UserWidget = InteractionPromptComponent->GetWidget();

	if (UserWidget)
	{
		PromptWidget = Cast<UDDPromptWidget>(UserWidget);
	}
}

void ADDGameEndActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TransitionTimer);
}

void ADDGameEndActor::EndGame(AActor* InterActor)
{
	APawn* Pawn = Cast<APawn>(InterActor);
	ADDPlayingGameMode* Mode = Cast<ADDPlayingGameMode>(GetWorld()->GetAuthGameMode());

	if ( Pawn && Mode )
	{
		Mode->EndGame(Pawn->GetPlayerState());
	}
}

void ADDGameEndActor::NetMulticastInteracted_Implementation()
{
	bIsInteracted = true;
	InteractionPromptComponent->SetVisibility(false);

	ADDPlayerController* PC = Cast<ADDPlayerController>(GetWorld()->GetFirstPlayerController());

	if ( PC )
	{
		PC->FadeOut(FadeDuration);
	}
}

void ADDGameEndActor::Interact(AActor* InterActor)
{
	if ( !CanInteract(InterActor) ) return;

	NetMulticastInteracted();

	FTimerDelegate GameEndDelegate = FTimerDelegate::CreateLambda([this, InterActor]()
		{
			EndGame(InterActor);
		});

	GetWorld()->GetTimerManager().SetTimer(
		TransitionTimer,
		GameEndDelegate,
		FadeDuration,
		false
	);
}

bool ADDGameEndActor::CanInteract(AActor* InterActor)
{
    return !bIsInteracted;
}

void ADDGameEndActor::BeginInteract(AActor* InterActor)
{
	APawn* Pawn = Cast<APawn>(InterActor);
	if ( (Pawn && !Pawn->IsLocallyControlled()) || !CanInteract(InterActor) )
	{
		return;
	}

	if (InteractionPromptComponent && PromptWidget)
	{
		PromptWidget->SetDescription(GetInteractionText(InterActor));
		InteractionPromptComponent->SetVisibility(true);
	}
	else
	{
		UE_LOG(LogDD, Error, TEXT("BeginInteract is failed"));
	}
}

void ADDGameEndActor::EndInteract(AActor* InterActor)
{
	APawn* Pawn = Cast<APawn>(InterActor);
	if (Pawn && !Pawn->IsLocallyControlled())
	{
		return;
	}

	if (InteractionPromptComponent)
	{
		InteractionPromptComponent->SetVisibility(false);
	}
}

FText ADDGameEndActor::GetInteractionText(AActor* InterActor)
{
    return InteractionText;
}

void ADDGameEndActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( !bIsInteracted ) return;

	FVector NowPos = GetActorLocation();
	SetActorLocation(NowPos + Launch * DeltaTime);
}