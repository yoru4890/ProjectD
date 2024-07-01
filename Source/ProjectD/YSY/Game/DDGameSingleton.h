// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDDGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UDDGameSingleton();
	static UDDGameSingleton& Get();
};
