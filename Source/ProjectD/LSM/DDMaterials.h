// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDMaterials.generated.h"

USTRUCT(BlueprintType)
struct FDDMaterials
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> Materials;
};