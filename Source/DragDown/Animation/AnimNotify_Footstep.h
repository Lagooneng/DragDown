// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Footstep.generated.h"

/**
 * 
 */
UCLASS()
class DRAGDOWN_API UAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundData")
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<class USoundBase>> SoundData;
};
