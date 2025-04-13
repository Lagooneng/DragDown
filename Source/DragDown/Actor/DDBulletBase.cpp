// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDBulletBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/DDCollision.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ADDBulletBase::ADDBulletBase()
{
	bReplicates = true;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->InitSphereRadius(5.0f);
	Trigger->SetCollisionProfileName(CPROFILE_DDTRIGGER);
	RootComponent = Trigger;
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADDBulletBase::OnComponentBeginOverlapCallback);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 3000.0f;
	Movement->MaxSpeed = 3000.0f;
	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;

	InitialLifeSpan = 3.0f;

	Power = 3000.0f;
}

void ADDBulletBase::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if ( HasAuthority() )
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			PushCharacter(Cast<ACharacter>(OtherActor));
		}
	}
}

void ADDBulletBase::PushCharacter(ACharacter* Character)
{
	if ( Character && Character->GetCharacterMovement() )
	{
		FVector Dir = GetActorForwardVector();
		Character->LaunchCharacter(Dir * Power, true, true);
	}
}



