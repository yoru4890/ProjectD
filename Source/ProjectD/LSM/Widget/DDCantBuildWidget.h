// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDCantBuildWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDCantBuildWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowCantPayImage();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowCantPlaceImage();
};
