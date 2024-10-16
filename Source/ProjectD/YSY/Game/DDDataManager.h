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
#include "LSM/Projectile/DDProjectileData.h"
#include "DDDataManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDDataManager : public UObject
{
	GENERATED_BODY()
	
public:
	UDDDataManager();

	FORCEINLINE TMap<FName, FDDEnemyData>& GetEnemyDataTable() { return EnemyDataTable; }
	FORCEINLINE TMap<FName, FDDTrapData>& GetTrapDataTable() { return TrapDataTable; }
	FORCEINLINE TMap<FName, FDDTowerData>& GetTowerDataTable() { return TowerDataTable; }
	FORCEINLINE TMap<FName, FDDWeaponData>& GetWeaponDataTable() { return WeaponDataTable; }
	FORCEINLINE TMap<FName, FDDWaveData>& GetWaveDataTable() { return WaveDataTable; }
	FORCEINLINE TMap<FName, FDDProjectileData>& GetProjectileDataTable() { return ProjectileDataTable; }

private:
	TMap<FName, FDDEnemyData> EnemyDataTable;
	TMap<FName, FDDTrapData> TrapDataTable;
	TMap<FName, FDDTowerData> TowerDataTable;
	TMap<FName, FDDWeaponData> WeaponDataTable;
	TMap<FName, FDDWaveData> WaveDataTable;
	TMap<FName, FDDProjectileData> ProjectileDataTable;


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
					//UE_LOG(LogDDGameSingleton, Warning, TEXT("Failed to find row: %s"), *RowName.ToString());
				}
			}
		}
		else
		{
			//UE_LOG(LogDDGameSingleton, Error, TEXT("Failed to load DataTable at path: %s"), *Path);
		}
	}
};
