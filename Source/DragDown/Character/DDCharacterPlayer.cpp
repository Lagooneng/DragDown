// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DDCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Physics/DDCollision.h"
#include "Player/DDPlayerState.h"
#include "DragDown.h"

ADDCharacterPlayer::ADDCharacterPlayer()
{
	bReplicates = true;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		MappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object; 
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// ASC
	ASC = nullptr;
}

UAbilitySystemComponent* ADDCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ADDCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetASC();
	SetGASAbilities();
}

void ADDCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump); 
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping); 
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::ShoulderLook); 

	SetupGASInputComponent();
}

void ADDCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetASC();
}

void ADDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 로컬 클라이언트만 밑으로 진입
	if (!IsLocallyControlled())
	{
		return;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void ADDCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(FollowCamera->GetForwardVector(), MovementVector.X);
	AddMovementInput(FollowCamera->GetRightVector(), MovementVector.Y);
}

void ADDCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ADDCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		//추가 gas 액션
		//EnhancedInputComponent->BindAction(~Action, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::GASInputPressed, 인덱스);

		UE_LOG(LogDD, Log, TEXT("SetupGASInputComponent Succeed"));
	}
	else if (!IsValid(ASC))
	{
		UE_LOG(LogDD, Log, TEXT("Invalid ASC"));
	}
	else
	{
		UE_LOG(LogDD, Log, TEXT("Invalid InputComponent"));
	}
}

void ADDCharacterPlayer::GASInputPressed(int32 InputId)
{
	if (HasAuthority())
	{
		HandleGASInputPressed(InputId);
	}
	else
	{
		ServerGASInputPressed(InputId);
	}
}

void ADDCharacterPlayer::ServerGASInputPressed_Implementation(int32 InputId)
{
	if (HasAuthority())
	{
		HandleGASInputPressed(InputId);
	}
}

void ADDCharacterPlayer::HandleGASInputPressed(int32 InputId)
{
	if (!ASC)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		if (Spec->InputPressed) return;
		//UE_LOG(LogCS, Log, TEXT("[NetMode : %d], HandleGASInputPressed"), GetWorld()->GetNetMode());
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void ADDCharacterPlayer::GASInputReleased(int32 InputId)
{
	if (HasAuthority())
	{
		HandleGASInputReleased(InputId);
	}
	else
	{
		ServerGASInputReleased(InputId);
	}
}

void ADDCharacterPlayer::HandleGASInputReleased(int32 InputId)
{
	if (!ASC)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void ADDCharacterPlayer::ServerGASInputReleased_Implementation(int32 InputId)
{
	HandleGASInputReleased(InputId);
}

void ADDCharacterPlayer::SetASC()
{
	if (ASC) return;

	ADDPlayerState* CSPS = GetPlayerState<ADDPlayerState>();
	if (CSPS)
	{
		ASC = CSPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(CSPS, this);
		ASC->ReplicationMode = EGameplayEffectReplicationMode::Mixed;
		UE_LOG(LogDD, Log, TEXT("*** [NetMode : %d] SetASC, %s, %s"), GetWorld()->GetNetMode(), *GetName(), *GetPlayerState()->GetName());
	}
	else
	{
		UE_LOG(LogDD, Log, TEXT("[NetMode %d] SetASC - ASC Not Found"), GetWorld()->GetNetMode());
	}
}

void ADDCharacterPlayer::SetGASAbilities()
{
	if (ASC)
	{
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}
	}
}