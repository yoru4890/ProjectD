// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameInstance.h"
#include "LSM/TrapManager/DDTrapAssetManager.h"
#include "LSM/TrapManager/DDTrapManager.h"

void UDDGameInstance::Init()
{
	Super::Init();
	TrapAssetManager = NewObject<UDDTrapAssetManager>(this);
	TrapManager = NewObject<UDDTrapManager>(this);
}
