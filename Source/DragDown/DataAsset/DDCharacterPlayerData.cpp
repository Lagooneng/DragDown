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

	CharacterPlayerData.TargetArmLength = 400.0f;
	CharacterPlayerData.CameraOffset = FVector();

	CharacterPlayerData.MeshLocation = FVector(0.0f, 0.0f, -100.0f);
	CharacterPlayerData.MeshRotation = FRotator(0.0f, -90.0f, 0.0f); 

	CharacterPlayerData.CapsuleRadius = 42.0f;
	CharacterPlayerData.CapsuleHeight = 96.0f;
}
