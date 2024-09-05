// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDUpgradeBuildingWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeBuildingSelected, FName, RowName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSellBuildingSelcted);
UCLASS()
class PROJECTD_API UDDUpgradeBuildingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, Category = "Events")
	FOnUpgradeBuildingSelected OnUpgradeBuildingSelected;

	UPROPERTY(BlueprintCallable, Category = "Events")
	FOnSellBuildingSelcted OnSellBuildingSelcted;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EBuildingType BuildingType;
	
};
