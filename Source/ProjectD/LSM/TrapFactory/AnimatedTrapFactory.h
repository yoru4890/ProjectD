// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TrapFactoryInterface.h"
#include "AnimatedTrapFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UAnimatedTrapFactory : public UObject, public ITrapFactoryInterface
{
	GENERATED_BODY()

public:
	virtual ADDTrapBase* CreateTrap(const FVector& Location, const FDDTrapStruct& TrapData, UWorld* World) override;
	
};
