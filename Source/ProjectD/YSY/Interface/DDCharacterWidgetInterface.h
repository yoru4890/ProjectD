// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DDCharacterWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDCharacterWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTD_API IDDCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void SetupCharacterWidget(class UDDUserWidget* InUserWidget) = 0;
	virtual void SetupRifleAmmoText(class UDDUserWidget* InUserWidget) {};
};
