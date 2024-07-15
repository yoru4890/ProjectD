// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YSY/GameData/DDEnemyData.h"
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

	FDDEnemyData GetEnemyData(const FName& EnemyName) const;

private:
	TMap<FName, FDDEnemyData> EnemyDataTable;
};
