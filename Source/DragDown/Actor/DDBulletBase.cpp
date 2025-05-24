// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDBulletBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/DDCollision.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Subsystem/DDNetworkObjectPoolingSubsystem.h"
#include "DragDown.h"

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
	Movement->ProjectileGravityScale = 0.0f;
	
	Power = 3000.0f;

	BulletLivingTime = 1.0f;
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

void ADDBulletBase::OnRetrievedFromPool()
{
	if ( HasAuthority() )
	{
		GetWorld()->GetTimerManager().SetTimer(PoolingTimer, this, &ADDBulletBase::PoolBullet, BulletLivingTime, false);

		NetMulticastOnRetrievedFromPool(GetActorLocation(), GetActorRotation());
	}
}

void ADDBulletBase::OnReturnedToPool()
{
	if ( HasAuthority() )
	{
		GetWorld()->GetTimerManager().ClearTimer(PoolingTimer);
		NetMulticastOnReturnedToPool();
	}
}

void ADDBulletBase::NetMulticastOnRetrievedFromPool_Implementation(FVector NewLocation, FRotator NewRotation)
{
	if (Movement)
	{
		Movement->StopMovementImmediately();
		Movement->Velocity = GetActorForwardVector() * Movement->InitialSpeed;
	}

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void ADDBulletBase::NetMulticastOnReturnedToPool_Implementation()
{
	if ( Movement )
	{
		Movement->StopMovementImmediately();
		Movement->Velocity = FVector();
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ADDBulletBase::PoolBullet()
{
	UDDNetworkObjectPoolingSubsystem* ObjectPool = GetWorld()->GetSubsystem<UDDNetworkObjectPoolingSubsystem>();
	if (ObjectPool)
	{
		ObjectPool->ReturnPooledObject(Cast<AActor>(this));
	}
}

void ADDBulletBase::FellOutOfWorld(const UDamageType& dmgType)
{
	PoolBullet(); 
}



