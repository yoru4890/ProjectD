// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDFactoryInterface.h"
#include "DDProjectileFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDProjectileFactory : public UObject, public IDDFactoryInterface
{
	GENERATED_BODY()

public:
	virtual UObject* CreateObject(const FDDFactoryParams& Params) override;
	
};
