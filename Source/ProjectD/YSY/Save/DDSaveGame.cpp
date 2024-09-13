// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Save/DDSaveGame.h"

void UDDSaveGame::SetBuildingLockState(const FName& BuildingName, const bool& bIsUnlocked)
{
	BuildingUnlocked.FindOrAdd(BuildingName, bIsUnlocked);
}
