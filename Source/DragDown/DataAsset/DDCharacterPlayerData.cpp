// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DDCharacterPlayerData.h"

UDDCharacterPlayerData::UDDCharacterPlayerData()
{
	CharacterPlayerData.RotationRate = FRotator(1500.0f, 500.0f, 0.0f);
	CharacterPlayerData.JumpZVelocity = 500.f;
	CharacterPlayerData.AirControl = 0.35f;
	CharacterPlayerData.MaxWalkSpeed = 500.f;
	CharacterPlayerData.MinAnalogWalkSpeed = 20.f;
	CharacterPlayerData.BrakingDecelerationWalking = 2000.f;
}
