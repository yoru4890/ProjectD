// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraFOVInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraFOVInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTD_API ICameraFOVInterface
{
	GENERATED_BODY()

public:
	virtual void SetCameraFOV(const float& Amount) = 0;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

};
