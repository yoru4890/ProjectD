// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameSingleton.h"

DEFINE_LOG_CATEGORY(LogDDGameSingleton);

UDDGameSingleton::UDDGameSingleton()
{
	LoadDataTable<FDDEnemyData>(EnemyDataTable, TEXT("/Script/Engine.DataTable'/Game/0000/YSY/Data/DT_EnemyData.DT_EnemyData'"));
	LoadDataTable<FDDTrapData>(TrapDataTable, TEXT("/Script/Engine.DataTable'/Game/0000/LSM/Data/LSM_DT_Trap.LSM_DT_Trap'"));
	LoadDataTable<FDDTowerData>(TowerDataTable, TEXT("/Script/Engine.DataTable'/Game/0000/LSM/Data/LSM_DT_Tower.LSM_DT_Tower'"));
	LoadDataTable<FDDWeaponData>(WeaponDataTable, TEXT("/Script/Engine.DataTable'/Game/0000/LJW/Blueprints/Data/LJW_DT_WeaponData.LJW_DT_WeaponData'"));
	LoadDataTable<FDDWaveData>(WaveDataTable, TEXT("/Script/Engine.DataTable'/Game/0000/YSY/Data/YSY_DT_WaveData.YSY_DT_WaveData'"));
}

UDDGameSingleton& UDDGameSingleton::Get()
{
	UDDGameSingleton* Singleton = CastChecked<UDDGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogDDGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UDDGameSingleton>();
}