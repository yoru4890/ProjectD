// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDFactoryInterface.h"
#include "LSM/Trap/DDTrapBase.h"
#include "DDTrapFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDTrapFactory : public UObject, public IDDFactoryInterface
{
	GENERATED_BODY()

public:
	virtual UObject* CreateObject(UWorld* World, const FName& RowName, const FDDBuildingBaseData& ObjectStruct, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator) override;
};
