// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSM/Building/DDBuildingBaseData.h"
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
	FORCEINLINE void SetBuildingType(EBuildingType InBuildingType) { BuildingType = InBuildingType; }

	FORCEINLINE void SetBuildingRowName(FName InRowName) { RowName = InRowName; }


	UPROPERTY(BlueprintCallable, Category = "Events")
	FOnUpgradeBuildingSelected OnUpgradeBuildingSelected;

	UPROPERTY(BlueprintCallable, Category = "Events")
	FOnSellBuildingSelcted OnSellBuildingSelcted;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EBuildingType BuildingType;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName RowName;
	
};
