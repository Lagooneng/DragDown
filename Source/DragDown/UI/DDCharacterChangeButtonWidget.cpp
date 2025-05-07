// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DDCharacterChangeButtonWidget.h"
#include "GameFramework/Character.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "DataAsset/DDMeshDatas.h"
#include "ActorComponent/DDMeshManagerComponent.h"
#include "DragDown.h"

void UDDCharacterChangeButtonWidget::NativeConstruct()
{
	if (BtnChangeCharacterAppearance)
	{
		BtnChangeCharacterAppearance->OnClicked.AddDynamic(this, &UDDCharacterChangeButtonWidget::OnClickedCallback);

		if (!CanChangeAppearance())
		{
			BtnChangeCharacterAppearance->SetIsEnabled(false);
		}
	}
}

void UDDCharacterChangeButtonWidget::SetMeshDescription(FText InDescription)
{
	if ( TxtChangeCharacterAppearance )
	{
		TxtChangeCharacterAppearance->SetText(InDescription);
	}
}

void UDDCharacterChangeButtonWidget::OnClickedCallback()
{
	if (!CanChangeAppearance()) return;

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if ( Character == nullptr )
	{
		UE_LOG(LogDD, Error, TEXT("No Player Character"));
		return;
	}

	if ( UDDMeshManagerComponent* MeshManager = Character->GetComponentByClass<UDDMeshManagerComponent>() )
	{
		MeshManager->SetMergedMesh(MeshID);
	}
}

bool UDDCharacterChangeButtonWidget::CanChangeAppearance()
{
	// 추후 서버와 연동하여 체크
	return true;
}
