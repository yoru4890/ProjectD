// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDFactoryInterface.h"
#include "LSM/Tower/DDTowerBase.h"
#include "DDTowerFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDTowerFactory : public UObject, public IDDFactoryInterface
{
	GENERATED_BODY()

public:
	virtual UObject* CreateObject(UWorld* World, const FName& RowName, const FDDBuildingBaseStruct& ObjectStruct, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator) override;
	
};
