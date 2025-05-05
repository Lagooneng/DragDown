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
#include "DataAsset/DDMeshDatas.h"
#include "ActorComponent/DDMeshManagerComponent.h"

ADDCharacterPlayer::ADDCharacterPlayer()
{
	bReplicates = true;

	NetPriority = 10.0f;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Capsule
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_DDCAPSULE);

	// Mesh
	GetMesh()->SetMobility(EComponentMobility::Movable);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	MeshPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshPivot"));
	MeshPivot->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(MeshPivot); 

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
	
	SetInputData();

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

void ADDCharacterPlayer::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (Data) 
	{
		// ENetowkSmoothingMode 때문에 Mesh Location, Rotation이 덮어써지는 문제가 있음
		// PreInitializeComponents에서 설정하면 Default Location, Rotation이 해당 값으로 설정됨
		SetData(); 
	}
}

void ADDCharacterPlayer::SetInputData()
{
	if (Data == nullptr) return;

	MappingContext = Data->CharacterPlayerData.MappingContext;
	JumpAction = Data->CharacterPlayerData.JumpAction;
	ShoulderMoveAction = Data->CharacterPlayerData.ShoulderMoveAction;
	ShoulderLookAction = Data->CharacterPlayerData.ShoulderLookAction;
}

void ADDCharacterPlayer::SetData()
{
	if (Data == nullptr) return;

	CameraBoom->TargetArmLength = Data->CharacterPlayerData.TargetArmLength;

	GetCharacterMovement()->RotationRate = Data->CharacterPlayerData.RotationRate;
	GetCharacterMovement()->JumpZVelocity = Data->CharacterPlayerData.JumpZVelocity;
	GetCharacterMovement()->AirControl = Data->CharacterPlayerData.AirControl;
	GetCharacterMovement()->MaxWalkSpeed = Data->CharacterPlayerData.MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = Data->CharacterPlayerData.MinAnalogWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = Data->CharacterPlayerData.BrakingDecelerationWalking;
	GetCapsuleComponent()->SetCapsuleSize(Data->CharacterPlayerData.CapsuleRadius, Data->CharacterPlayerData.CapsuleHeight);

	GetMesh()->SetSkeletalMesh(Data->CharacterPlayerData.Mesh);
	GetMesh()->SetAnimInstanceClass(Data->CharacterPlayerData.AnimInstance);
	GetMesh()->SetRelativeLocation(Data->CharacterPlayerData.MeshLocation);
	GetMesh()->SetRelativeRotation(Data->CharacterPlayerData.MeshRotation);
	MeshManagerComponent->SetMergedMesh(EMESHID::DRESS);
	UE_LOG(LogDD, Log, TEXT("[NetMode: %d] SetData - Mesh Z Pos : %f"), GetWorld()->GetNetMode(), GetMesh()->GetRelativeLocation().Z);
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