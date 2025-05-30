// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DDCharacterPatrol.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/DDCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "AI/Controller/DDAIController.h"
#include "Physics/DDCollision.h"
#include "AbilitySystemComponent.h"
#include "DragDown.h"
#include "Tag/DDTag.h"

ADDCharacterPatrol::ADDCharacterPatrol()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;

	CurrentPatrolIdx = 0;
	PatrolPosesLength = 0;

	// Player Sensor
	PlayerSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerSensor"));
	PlayerSensor->SetBoxExtent(FVector(800.0f, 700.0f, 130.0f));
	PlayerSensor->SetCollisionProfileName( CPROFILE_DDTRIGGER );
	PlayerSensor->SetupAttachment(GetCapsuleComponent());
	PlayerSensor->SetRelativeLocation(FVector(700.0f, 0.0f, 130.0f));
	PlayerSensor->OnComponentBeginOverlap.AddDynamic(this, &ADDCharacterPatrol::OnTriggerBeginOverlap);
	PlayerSensor->OnComponentEndOverlap.AddDynamic(this, &ADDCharacterPatrol::OnTriggerEndOverlap);

	Power = 2000.0f;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
}

void ADDCharacterPatrol::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PatrolPosesLength = PatrolPoses.Num();
	}

	ASC->AddLooseGameplayTag(DDTAG_STATE_DODGE);
}

UAbilitySystemComponent* ADDCharacterPatrol::GetAbilitySystemComponent() const
{
	return ASC; 
}

ADDCharacterPlayer* ADDCharacterPatrol::GetCharacterPlayer()
{
	return CharacterPlayer; 
}

FVector ADDCharacterPatrol::GetPatrolPos()
{
	if (PatrolPosesLength == 0)
	{
		return GetActorLocation(); 
	}

	int8 PrevIdx = CurrentPatrolIdx;
	CurrentPatrolIdx = (CurrentPatrolIdx + 1) % PatrolPosesLength;

	if (PatrolPoses.IsValidIndex(PrevIdx))
	{
		UE_LOG(LogDD, Log, TEXT("%d %d"), CurrentPatrolIdx, PatrolPosesLength);
		return PatrolPoses[PrevIdx];
	}
	else
	{
		return GetActorLocation(); 
	}
}

void ADDCharacterPatrol::PushCharacter()
{
	if ( CharacterPlayer )
	{
		FVector Dir = GetActorForwardVector();
		CharacterPlayer->LaunchCharacter(Dir * Power, true, true);
	}
}

void ADDCharacterPatrol::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if ( HasAuthority() )
	{
		ADDCharacterPlayer* OverlappedPlayer = Cast<ADDCharacterPlayer>(OtherActor);

		if (OverlappedPlayer)
		{
			CharacterPlayer = OverlappedPlayer;
			Cast<ADDAIController>(GetController())->ActiveMove(false);
		}
	}
}

void ADDCharacterPatrol::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		ADDCharacterPlayer* OverlappedPlayer = Cast<ADDCharacterPlayer>(OtherActor);

		if (OverlappedPlayer) 
		{
			Cast<ADDAIController>(GetController())->ActiveMove(true); 
			CharacterPlayer = nullptr;
		}
	}
}
