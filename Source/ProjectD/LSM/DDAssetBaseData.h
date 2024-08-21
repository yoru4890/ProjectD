// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDAssetBaseData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDDAssetBaseData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName; // 오브젝트의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectInfo; // 오브젝트의 정보
};
