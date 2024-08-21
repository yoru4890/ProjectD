// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDEffectSystemComponent.generated.h"

// TODO : YSY Effect ObjectPool

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UDDEffectSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDDEffectSystemComponent();

protected:
	virtual void BeginPlay() override;

public:	

};
