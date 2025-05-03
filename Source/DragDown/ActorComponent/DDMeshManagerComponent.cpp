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
		UE_LOG(LogDD, Warning, TEXT("Invalid MeshID"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	FSkeletalMeshMergeParams MergeParams;
	MergeParams.MeshesToMerge = MeshDatas->Meshes.Find(MeshID)->Get()->MeshesToMerge;
	MergeParams.Skeleton = MeshDatas->Meshes.Find(MeshID)->Get()->Skeleton;
	MergeParams.StripTopLODS = 0;
	MergeParams.bSkeletonBefore = true;
	MergeParams.bNeedsCpuAccess = true;

	USkeletalMesh* MergedMesh = USkeletalMergingLibrary::MergeMeshes(MergeParams);
	MergedMesh->SetPhysicsAsset(MeshDatas->Meshes.Find(MeshID)->Get()->PhysicsAsset);
	Character->GetMesh()->SetSkeletalMesh(MergedMesh);

	SetHairMesh(MeshID);
}

void UDDMeshManagerComponent::SetHairMesh(EMESHID MeshID)
{
	if (MeshDatas->Meshes.Find(MeshID)->Get()->HairMesh == nullptr)
	{
		return;
	}
	ADDCharacterBase* CharacterBase = Cast<ADDCharacterBase>(GetOwner());

	if ( CharacterBase )
	{
		CharacterBase->GetHairMeshComponent()->SetSkeletalMesh(MeshDatas->Meshes.Find(MeshID)->Get()->HairMesh);
		CharacterBase->GetHairMeshComponent()->SetMasterPoseComponent(CharacterBase->GetMesh());
	}
}


