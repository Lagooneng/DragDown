// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DDUserAuthSubsystem.h"

UDDUserAuthSubsystem::UDDUserAuthSubsystem()
{
	ClearSession();
}

void UDDUserAuthSubsystem::ClearSession()
{
	Token.Empty();
	UserName.Empty();
}
