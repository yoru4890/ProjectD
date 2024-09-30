// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "DDTrapData.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class ETrapType : uint8
{
	ThornTrap UMETA(DisplayName = "ThornTrap"),
	ElementalTrap UMETA(DisplayName = "ElementalTrap"),
	Unknown
};

USTRUCT(BlueprintType)
struct FDDTrapData : public FDDBuildingBaseData
{
	GENERATED_BODY()

public:
	FDDTrapData() : TrapType(ETrapType::Unknown) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADDTrap> TrapClass; // Æ®·¦ Å¬·¡½º

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETrapType TrapType;
};