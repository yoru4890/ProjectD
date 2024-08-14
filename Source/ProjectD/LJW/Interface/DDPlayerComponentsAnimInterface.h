// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DDPlayerComponentsAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDPlayerComponentsAnimInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTD_API IDDPlayerComponentsAnimInterface
{
	GENERATED_BODY()

public:

	virtual class UDDWeaponSystemComponent* GetWeaponComp() = 0;
};
