// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_Footstep.h"
#include "ActorComponent/DDSurfaceDetectionComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "DragDown.h"

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr) return;

	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr) return;

	UDDSurfaceDetectionComponent* SurfaceDetectionComponent = Owner->FindComponentByClass<UDDSurfaceDetectionComponent>();
	if (SurfaceDetectionComponent == nullptr) return;


	EPhysicalSurface Surface = SurfaceDetectionComponent->GetCurrentPhysicalSurface();

	if ( SoundData.Contains(Surface) )
	{
		if ( USoundBase* Sound = SoundData[Surface] )
		{
			UE_LOG(LogDD, Log, TEXT("UAnimNotify_Footstep Notify USoundBase"));

			UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), Sound, Owner->GetActorLocation());
		}
	}
}