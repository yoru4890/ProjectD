// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDFactoryInterface.h"
#include "DDTowerFactory.generated.h"

/**
 *
 */
UCLASS()
class PROJECTD_API UDDTowerFactory : public UObject, public IDDFactoryInterface
{
	GENERATED_BODY()

public:
	virtual UObject* CreateObject(UWorld* World, const FName& RowName, AActor* Owner, APawn* Instigator) override;

};
