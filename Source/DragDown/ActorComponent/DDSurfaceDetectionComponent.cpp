// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DDSurfaceDetectionComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAsset/DDGroundFrictionData.h"
#include "Physics/DDCollision.h"

UDDSurfaceDetectionComponent::UDDSurfaceDetectionComponent()
{
	SetIsReplicatedByDefault(true);
}

void UDDSurfaceDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr) return;

	CharacterMovementComp = OwnerCharacter->GetCharacterMovement();
	if (CharacterMovementComp == nullptr) return;

	OwnerCharacter->OnCharacterMovementUpdated.AddDynamic(this, &UDDSurfaceDetectionComponent::OnMovementUpdated);
}


void UDDSurfaceDetectionComponent::OnMovementUpdated(float DeltaTime, FVector OldLocation, FVector OldVelocity)
{
	if (CharacterMovementComp == nullptr || GroundFrictionData == nullptr) return;

	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 200.f);

	FHitResult Hit;  
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	Params.bReturnPhysicalMaterial = true; // CurrentFloor.HitResult is not support bReturnPhysicalMaterial

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		if (UPhysicalMaterial* PhysMat = Hit.PhysMaterial.Get())
		{
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMat);

			switch (SurfaceType)
			{
			case SURFACE_ICE:
				SetMovementForFriction(SURFACE_ICE);
				break;
			default:
				SetMovementForFriction(SURFACE_DEFAULT);
				break;
			}
		}
	}
}

void UDDSurfaceDetectionComponent::SetMovementForFriction(EPhysicalSurface NewSurface)
{
	CharacterMovementComp->GroundFriction = GroundFrictionData->FrictionData[NewSurface].GroundFriction;
	CharacterMovementComp->BrakingFrictionFactor = GroundFrictionData->FrictionData[NewSurface].BrakingFrictionFactor;
	CharacterMovementComp->BrakingDecelerationWalking = GroundFrictionData->FrictionData[NewSurface].BrakingDecelerationWalking;
}


