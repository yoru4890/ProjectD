// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Manager/DDBuildingManager.h"
#include "LSM/Manager/DDFactoryManager.h"
#include "YSY/Manager/DDEnemySpawnManager.h"
#include "YSY/Manager/DDWaveManager.h"

void UDDGameInstance::Init()
{
	Super::Init();
	BuildingManager = NewObject<UDDBuildingManager>(this);
	FactoryManager = NewObject<UDDFactoryManager>(this);
	AssetManager = NewObject<UDDAssetManager>(this);

	BuildingManager->Initialize();
	FactoryManager->Initialize(BuildingManager->GetBuildingDataTable());
	AssetManager->Initialize();
	
	EnemySpawnManager = NewObject<UDDEnemySpawnManager>(this);
	WaveManager = NewObject<UDDWaveManager>(this);

	WaveManager->Initialize(this);
}
