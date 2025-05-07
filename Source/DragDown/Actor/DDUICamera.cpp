// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDUICamera.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ADDUICamera::ADDUICamera()
{
	PrimaryActorTick.bCanEverTick = true;

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	SetRootComponent(Capture);
	Offset = FVector(-100.0f, 0.0f, 7.0f);
}

void ADDUICamera::BeginPlay()
{
	Super::BeginPlay();

	Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ADDUICamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector StartLocation = Character->GetActorLocation() + Offset;
	FVector TargetLocation = Character->GetActorLocation();
	SetActorLocation(StartLocation);

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	SetActorRotation(LookAtRotation);
}

