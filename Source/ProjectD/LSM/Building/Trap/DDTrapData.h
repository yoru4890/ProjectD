// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "DDTrapData.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FDDTrapData : public FDDBuildingBaseData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADDTrap> TrapClass; // Æ®·¦ Å¬·¡½º
};