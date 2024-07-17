// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDTrapFactoryInterface.h"
#include "DDAnimatedTrapFactory.generated.h"

/**
 *
 */
UCLASS()
class PROJECTD_API UDDAnimatedTrapFactory : public UObject, public IDDTrapFactoryInterface
{
	GENERATED_BODY()

public:
	virtual ADDTrapBase* CreateTrap(UWorld* World, const FName& TrapName, const FDDTrapStruct& TrapStruct, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator) override;

};
