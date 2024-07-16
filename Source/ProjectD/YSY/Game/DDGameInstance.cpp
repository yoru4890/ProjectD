// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameInstance.h"
#include "LSM/TrapManager/DDTrapAssetManager.h"
#include "LSM/TrapManager/DDTrapManager.h"
#include "LSM/TrapManager/DDTrapFactoryManager.h"

void UDDGameInstance::Init()
{
	Super::Init();
	TrapManager = NewObject<UDDTrapManager>(this);
	TrapFactoryManager = NewObject<UDDTrapFactoryManager>(this);
	TrapAssetManager = NewObject<UDDTrapAssetManager>(this);

	TrapFactoryManager->Initialize(TrapManager->GetTrapDataTable());
	TrapAssetManager->LoadTrapAssetsAsync();

}
