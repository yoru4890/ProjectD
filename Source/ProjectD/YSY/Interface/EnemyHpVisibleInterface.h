// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyHpVisibleInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyHpVisibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTD_API IEnemyHpVisibleInterface
{
	GENERATED_BODY()
public:

	virtual void SetVisibleHpBar(bool bIsVisible) = 0;
};
