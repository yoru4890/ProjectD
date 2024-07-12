// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Trap/DDTrapStruct.h"
#include "UObject/NoExportTypes.h"
#include "DDTrapFactoryManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDTrapFactoryManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Trap")
	static ADDTrapBase* CreateTrap(ETrapType TrapType, const FVector& Location, const FDDTrapStruct& TrapData, UWorld* World);
};
