// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Save/DDSaveGame.h"

UDDSaveGame::UDDSaveGame()
{
	const int32 NumStages = 10;

	StageLikePoints.SetNum(NumStages);
	StageCleared.SetNum(NumStages);

	for (int32 i{}; i < NumStages; i++)
	{
		StageLikePoints[i] = 0;
		StageCleared[i] = false;
	}
}

void UDDSaveGame::SetBuildingLockState(const FName& BuildingName, const bool& bIsUnlocked)
{
	BuildingUnlocked.FindOrAdd(BuildingName, bIsUnlocked);
}

void UDDSaveGame::SetStageLikePoint(const int32& CurrentStage, const int32& LikePoint)
{
	StageLikePoints[CurrentStage] = std::max(StageLikePoints[CurrentStage],LikePoint);
}

void UDDSaveGame::SetStageCleared(const int32& CurrentStage, const bool& bIsCleared)
{
	StageCleared[CurrentStage] = bIsCleared || StageCleared[CurrentStage];
}
