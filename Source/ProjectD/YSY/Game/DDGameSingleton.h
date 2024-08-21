// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YSY/GameData/DDEnemyData.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "LSM/Building/Trap/DDTrapData.h"
#include "LSM/Building/Tower/DDTowerData.h"
#include "LJW/Weapon/DDWeaponData.h"
#include "YSY/GameData/DDWaveData.h"
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
	FORCEINLINE TMap<FName, FDDTrapData>& GetTrapDataTable() { return TrapDataTable; }
	FORCEINLINE TMap<FName, FDDTowerData>& GetTowerDataTable() { return TowerDataTable; }
	FORCEINLINE TMap<FName, FDDWeaponData>& GetWeaponDataTable() { return WeaponDataTable; }
	FORCEINLINE TMap<FName, FDDWaveData>& GetWaveDataTable() { return WaveDataTable; }

private:
	TMap<FName, FDDEnemyData> EnemyDataTable;
	TMap<FName, FDDTrapData> TrapDataTable;
	TMap<FName, FDDTowerData> TowerDataTable;
	TMap<FName, FDDWeaponData> WeaponDataTable;
	TMap<FName, FDDWaveData> WaveDataTable;


	template<typename T>
	void LoadDataTable(TMap<FName, T>& DataTable, const FString& Path)
	{
		ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(*Path);
		if (DataTableRef.Object)
		{
			const UDataTable* DataTableObject = DataTableRef.Object;
			check(DataTableObject->GetRowMap().Num() > 0);

			TArray<FName> RowNames = DataTableObject->GetRowNames();
			for (const FName& RowName : RowNames)
			{
				T* RowData = DataTableObject->FindRow<T>(RowName, "");
				if (RowData)
				{
					DataTable.Add(RowName, *RowData);
				}
				else
				{
					UE_LOG(LogDDGameSingleton, Warning, TEXT("Failed to find row: %s"), *RowName.ToString());
				}
			}
		}
		else
		{
			UE_LOG(LogDDGameSingleton, Error, TEXT("Failed to load DataTable at path: %s"), *Path);
		}
	}
};
