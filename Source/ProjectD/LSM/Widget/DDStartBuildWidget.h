// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "DDStartBuildWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingTypeSelected, EBuildingType, BuildingType);

UCLASS()
class PROJECTD_API UDDStartBuildWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, Category = "Events")
	FOnBuildingTypeSelected OnBuildingTypeSelected;
	
};
