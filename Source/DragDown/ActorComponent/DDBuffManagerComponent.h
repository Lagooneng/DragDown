// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDBuffManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGDOWN_API UDDBuffManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDDBuffManagerComponent();

protected:
	void Initailize(class UAbilitySystemComponent* InASC);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory", meta = (AllowPrivateAccess = "true"))
	TArray< TSubclassOf<class UGameplayEffect> > InitBuffs;

	TObjectPtr<class UAbilitySystemComponent> ASC;
};
