// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DDCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/DDCollision.h"
#include "ActorComponent/DDAttackStateComponent.h"
#include "DragDown.h"

// Sets default values
ADDCharacterBase::ADDCharacterBase()
{
	bReplicates = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1500.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Attack State 관리, NPC가 사용 가능하니 Base에 설정
	AttackStateComponent = CreateDefaultSubobject<UDDAttackStateComponent>(TEXT("AttackStateComponent"));
}

void ADDCharacterBase::NetMulticastPlayAnimMontage_Implementation(UAnimMontage* Montage, FName SectionName) 
{
	//if (IsLocallyControlled()) return;
	UE_LOG(LogDD, Display, TEXT("[NetMode : %d] NetMulticastPlayAnimMontage_Implementation"), GetWorld()->GetNetMode());
	if ( GetMesh() && GetMesh()->GetAnimInstance() )
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage, 1.0f);
		if (!SectionName.IsNone()) 
		{
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
		}
	}
}

