// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AggroTargetInterface.generated.h"

UINTERFACE(Blueprintable)
class UAggroTargetInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTD_API IAggroTargetInterface
{
	GENERATED_BODY()

public:
	virtual void AddAggro() = 0;
	virtual void MaxAggro() = 0;
};
