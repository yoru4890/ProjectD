// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DDBuildingAttackStrategyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDBuildingAttackStrategyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTD_API IDDBuildingAttackStrategyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Attack(AActor* TargetEnemy) = 0;
};
