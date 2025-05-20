// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/DDAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/DDUserAuthSubsystem.h"

ADDPlayerState::ADDPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UDDAttributeSet>(TEXT("AttributeSet"));
}

void ADDPlayerState::ServerSetUserName_Implementation(const FString& InUserName)
{
	if ( HasAuthority() )
	{
		UserName = InUserName;
		OnPlayerInfoChanged.Broadcast();
	}
}

UAbilitySystemComponent* ADDPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

void ADDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UDDUserAuthSubsystem* UserAuthSubsystem = GetGameInstance()->GetSubsystem<UDDUserAuthSubsystem>();
	if (UserAuthSubsystem == nullptr) return;

	if (HasAuthority())
	{
		UserName = UserAuthSubsystem->GetUserName(); 
		OnPlayerInfoChanged.Broadcast(); 
	}
	else
	{
		ServerSetUserName(UserAuthSubsystem->GetUserName());
	}
}

void ADDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDPlayerState, UserName);
}

void ADDPlayerState::OnRep_UserName()
{
	OnPlayerInfoChanged.Broadcast();
}
