// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DDGimmickActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DragDown.h"

// Sets default values
ADDGimmickActor::ADDGimmickActor()
{
    bReplicates = true;

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
    RootComponent = StaticMeshComp;

    ActionTime = 2.0f;
    NowPos = 0;
    NowRot = 0;
}

void ADDGimmickActor::BeginPlay()
{
    Super::BeginPlay();

    ActGimmick();
}

void ADDGimmickActor::ActGimmick()
{
    UE_LOG(LogDD, Log, TEXT("ActGimmick"));

    FVector TargetPos;
    FRotator TargetRot;

    if (TargetPositions.Num() == 0)
    {
        TargetPos = GetActorLocation();
    }
    else
    {
        TargetPos = TargetPositions[NowPos];
    }

    if (TargetRotations.Num() == 0)
    {
        TargetRot = GetActorRotation();
    }
    else
    {
        TargetRot = TargetRotations[NowRot];
    }

    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;
    LatentInfo.ExecutionFunction = FName("OnActionFinished");
    LatentInfo.Linkage = 0;
    LatentInfo.UUID = __LINE__;

    UKismetSystemLibrary::MoveComponentTo(
        StaticMeshComp,
        TargetPos,
        TargetRot,
        true,
        true,
        ActionTime,
        false,
        EMoveComponentAction::Move,
        LatentInfo
    );
}

void ADDGimmickActor::OnActionFinished()
{
    UE_LOG(LogDD, Log, TEXT("OnActionFinished"));
    if (TargetPositions.Num() != 0)
    {
        NowPos = ++NowPos % TargetPositions.Num();
    }

    if (TargetRotations.Num() != 0)
    {
        NowRot = ++NowRot % TargetRotations.Num();
    }

    ActGimmick();
}


