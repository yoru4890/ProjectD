// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YSY/GameData/DDEnemyData.h"
#include "LSM/Trap/DDTrapStruct.h"
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

	FORCEINLINE TMap<FName, FDDEnemyData>& GetEnemyDataTable() { return EnemyDataTable; }
	FORCEINLINE TMap<FName, FDDTrapStruct>& GetTrapDataTable() { return TrapDataTable; }

private:
	TMap<FName, FDDEnemyData> EnemyDataTable;
	TMap<FName, FDDTrapStruct> TrapDataTable;


	template<typename T>
	void LoadDataTable(TMap<FName, T>& DataTable, const FString& Path);
};
