// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DDCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/DDCollision.h"
#include "ActorComponent/DDAttackStateComponent.h"
#include "ActorComponent/DDMeshManagerComponent.h"
#include "DragDown.h"

// Sets default values
ADDCharacterBase::ADDCharacterBase()
{
	bReplicates = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Attack State 관리, NPC가 사용 가능하니 Base에 설정
	AttackStateComponent = CreateDefaultSubobject<UDDAttackStateComponent>(TEXT("AttackStateComponent"));

	MeshManagerComponent = CreateDefaultSubobject<UDDMeshManagerComponent>(TEXT("MeshManagerComponent"));
	HairSKMComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMeshComponent"));
	HairSKMComponent->SetupAttachment(GetMesh());

	bIsActionEnabled = true;

	// Trigger
	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->InitCapsuleSize(50.0f, 100.0f);
	Trigger->SetCollisionProfileName(CPROFILE_OVERLAPALL);
}

void ADDCharacterBase::ClientSetActionEnabled_Implementation(bool bInActionEnabled)
{
	if ( IsLocallyControlled() )
	{
		bIsActionEnabled = bInActionEnabled; 
	}
}

void ADDCharacterBase::SetActionEnabled(bool bInActionEnabled)
{
	bIsActionEnabled = bInActionEnabled;

	if ( HasAuthority() )
	{
		ClientSetActionEnabled(bInActionEnabled);
	}
}


void ADDCharacterBase::Jump()
{
	if (!bIsActionEnabled) return;

	Super::Jump();
}

UCapsuleComponent* ADDCharacterBase::GetTrigger()
{
	return Trigger;
}

void ADDCharacterBase::NetMulticastPlayOtherClientMontage_Implementation(UAnimMontage* Montage, FName SectionName)
{
	if (IsLocallyControlled()) return;

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

