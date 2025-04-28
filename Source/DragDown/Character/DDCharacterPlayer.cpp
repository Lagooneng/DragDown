// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DDCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Physics/DDCollision.h"
#include "ActorComponent/DDBuffManagerComponent.h"
#include "ActorComponent/DDSurfaceDetectionComponent.h"
#include "ActorComponent/DDGASManagerComponent.h"
#include "DataAsset/DDCharacterPlayerData.h"
#include "DragDown.h"


ADDCharacterPlayer::ADDCharacterPlayer()
{
	bReplicates = true;

	NetPriority = 10.0f;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_DDCAPSULE);

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	BuffManagerComponent = CreateDefaultSubobject<UDDBuffManagerComponent>(TEXT("BuffManagerComponent"));

	SurfaceDetectionComponent = CreateDefaultSubobject<UDDSurfaceDetectionComponent>(TEXT("SurfaceDetectionComponent"));

	GASManagerComponent = CreateDefaultSubobject<UDDGASManagerComponent>(TEXT("GASManagerComponent"));
}

void ADDCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GASManagerComponent->SetASC();
	GASManagerComponent->SetGASAbilities();
	BuffManagerComponent->Initailize();
}

void ADDCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogDD, Log, TEXT("SetupPlayerInputComponent"));
	
	SetData();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(MappingContext, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::Jump);  
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping); 
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::ShoulderLook); 

	GASManagerComponent->SetupGASInputComponent();
}

void ADDCharacterPlayer::SetData()
{
	if (Data == nullptr) return;

	GetCharacterMovement()->RotationRate = Data->CharacterPlayerData.RotationRate;
	GetCharacterMovement()->JumpZVelocity = Data->CharacterPlayerData.JumpZVelocity;
	GetCharacterMovement()->AirControl = Data->CharacterPlayerData.AirControl;
	GetCharacterMovement()->MaxWalkSpeed = Data->CharacterPlayerData.MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = Data->CharacterPlayerData.MinAnalogWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = Data->CharacterPlayerData.BrakingDecelerationWalking;

	MappingContext = Data->CharacterPlayerData.MappingContext;
	JumpAction = Data->CharacterPlayerData.JumpAction;
	ShoulderMoveAction = Data->CharacterPlayerData.ShoulderMoveAction;
	ShoulderLookAction = Data->CharacterPlayerData.ShoulderLookAction;

	GetMesh()->SetSkeletalMesh(Data->CharacterPlayerData.Mesh);
	GetMesh()->SetAnimInstanceClass(Data->CharacterPlayerData.AnimInstance);
}

void ADDCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	GASManagerComponent->SetASC();
	BuffManagerComponent->Initailize();
}

void ADDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogDD, Log, TEXT("BeginPlay"));
	
	// Setting for other client
	if ( !IsLocallyControlled() )
	{
		SetData();
	}
}

void ADDCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	if (!bIsActionEnabled) return;

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
