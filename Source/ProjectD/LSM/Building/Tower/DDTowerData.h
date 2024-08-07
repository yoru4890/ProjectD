// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "DDTowerData.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FDDTowerData : public FDDBuildingBaseData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADDTower> TowerClass; // 타워 클래스
};
