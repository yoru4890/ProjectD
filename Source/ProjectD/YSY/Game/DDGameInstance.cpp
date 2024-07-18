// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Manager/DDTrapManager.h"
#include "LSM/Manager/DDFactoryManager.h"

void UDDGameInstance::Init()
{
	Super::Init();
	TrapManager = NewObject<UDDTrapManager>(this);
	FactoryManager = NewObject<UDDFactoryManager>(this);
	AssetManager = NewObject<UDDAssetManager>(this);

	FactoryManager->Initialize(TrapManager->GetTrapDataTable());
	AssetManager->Initialize();
	

}
