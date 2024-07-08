// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDTowerManager.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTowerStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TowerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TowerCost;
};

UCLASS()
class PROJECTD_API UDDTowerManager : public UObject
{
	GENERATED_BODY()
	
};
