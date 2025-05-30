// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/DDMeshManagerComponent.h"
#include "DataAsset/DDMeshDatas.h"
#include "DataAsset/DDMeshesToMergeData.h"
#include "SkeletalMergingLibrary.h"
#include "GameFramework/Character.h"
#include "Character/DDCharacterBase.h"
#include "DragDown.h"

UDDMeshManagerComponent::UDDMeshManagerComponent()
{
	MeshIDCache = EMESHID::NONE;
	LoadConfig(); 
}

void UDDMeshManagerComponent::LoadCachedMesh()
{
	LoadConfig();
	SetMergedMesh(MeshIDCache); 
}

void UDDMeshManagerComponent::SetMergedMesh(EMESHID MeshID)
{
	if (!MeshDatas)
	{
		UE_LOG(LogDD, Warning, TEXT("No Meshes Data"));
		return;
	}

	if (!MeshDatas->Meshes.Contains(MeshID))
	{
		UE_LOG(LogDD, Warning, TEXT("Invalid MeshID : %s, %d"), *UEnum::GetValueAsString(MeshID), MeshID);
		return;
	}

	if (MeshDatas->Meshes.Find(MeshID)->Get() == nullptr)
	{
		UE_LOG(LogDD, Warning, TEXT("Invalid Mesh Data"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if ( MeshDatas->Meshes.Find(MeshID)->Get()->MeshesToMerge.Num() > 2)
	{
		FSkeletalMeshMergeParams MergeParams;
		MergeParams.MeshesToMerge = MeshDatas->Meshes.Find(MeshID)->Get()->MeshesToMerge;
		MergeParams.Skeleton = MeshDatas->Meshes.Find(MeshID)->Get()->Skeleton;
		MergeParams.StripTopLODS = 0;
		MergeParams.bSkeletonBefore = true;
		MergeParams.bNeedsCpuAccess = true;

		USkeletalMesh* MergedMesh = USkeletalMergingLibrary::MergeMeshes(MergeParams);
		MergedMesh->SetPhysicsAsset(MeshDatas->Meshes.Find(MeshID)->Get()->PhysicsAsset);
		Character->GetMesh()->SetSkeletalMesh(MergedMesh);
	}
	if (MeshDatas->Meshes.Find(MeshID)->Get()->MeshesToMerge.Num() == 1)
	{
		Character->GetMesh()->SetSkeletalMesh(MeshDatas->Meshes.Find(MeshID)->Get()->MeshesToMerge[0]);
	}

	if (MeshDatas->Meshes.Find(MeshID)->Get()->AnimInstance)
	{
		Character->GetMesh()->SetAnimInstanceClass(MeshDatas->Meshes.Find(MeshID)->Get()->AnimInstance);
	}

	SetHairMesh(MeshID);

	if (MeshID != EMESHID::NONE)
	{
		UE_LOG(LogDD, Log, TEXT("SetMergedMesh Caching - %s"), *UEnum::GetValueAsString(MeshID));
		MeshIDCache = MeshID;
		SaveConfig();
	}
}

void UDDMeshManagerComponent::SetHairMesh(EMESHID MeshID)
{
	ADDCharacterBase* CharacterBase = Cast<ADDCharacterBase>(GetOwner());

	if ( CharacterBase )
	{
		CharacterBase->GetHairMeshComponent()->SetSkeletalMesh(MeshDatas->Meshes.Find(MeshID)->Get()->HairMesh);
		CharacterBase->GetHairMeshComponent()->SetLeaderPoseComponent(CharacterBase->GetMesh());
	}
}


