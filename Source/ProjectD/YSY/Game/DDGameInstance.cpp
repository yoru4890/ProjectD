// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Manager/DDBuildingManager.h"
#include "LSM/Manager/DDFactoryManager.h"
#include "YSY/Manager/DDEnemySpawnManager.h"
#include "YSY/Manager/DDWaveManager.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDDataManager.h"
#include "LJW/Manager/DDSoundManager.h"

UDDGameInstance::UDDGameInstance()
{
	
}

void UDDGameInstance::Init()
{
	Super::Init();
	DataManager = NewObject<UDDDataManager>(this);
	FactoryManager = NewObject<UDDFactoryManager>(this);
	BuildingManager = NewObject<UDDBuildingManager>(this);
	ProjectileManager = NewObject<UDDProjectileManager>(this);
	AssetManager = NewObject<UDDAssetManager>(this);
	SoundManager = NewObject<UDDSoundManager>(this);

	AssetManager->Initialize();
	FactoryManager->Initialize();
	ProjectileManager->Initialize();
	BuildingManager->Initialize();

	
	EnemySpawnManager = NewObject<UDDEnemySpawnManager>(this);
	WaveManager = NewObject<UDDWaveManager>(this);

	WaveManager->Initialize();
	InitializeManagerDelegates();

	//Sound
}


void UDDGameInstance::InitializeManagerDelegates()
{
	WaveManager->OnActivateEnemySignature.BindUObject(EnemySpawnManager, &UDDEnemySpawnManager::Activate);
	WaveManager->OnSetupEnemyPoolSignature.BindUObject(EnemySpawnManager, &UDDEnemySpawnManager::SetupEnemyPools);

	EnemySpawnManager->OnAddEnemySignature.BindUObject(WaveManager, &UDDWaveManager::AddEnemyNumber);
	EnemySpawnManager->OnSubEnemySignature.BindUObject(WaveManager, &UDDWaveManager::SubEnemyNumber);

}



