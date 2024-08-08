// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/DDWaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/AI/AISplineRoute.h"
#include "YSY/Manager/DDEnemySpawnManager.h"
#include "YSY/Game/DDGameInstance.h"

UDDWaveManager::UDDWaveManager()
{
	ConstructorHelpers::FObjectFinder<UDataTable> WaveDataTableRef(TEXT("/Script/Engine.DataTable'/Game/0000/YSY/Data/YSY_DT_WaveData.YSY_DT_WaveData'"));

	if (WaveDataTableRef.Object)
	{
		const UDataTable* DataTableObject = WaveDataTableRef.Object;
		check(DataTableObject->GetRowMap().Num() > 0);

		StageWaveInfo.SetNum(DataTableObject->GetRowMap().Num() + 1);

		auto& DataMap = DataTableObject->GetRowMap();
		for (auto& [RowName, Data] : DataMap)
		{
			auto WaveData = reinterpret_cast<FDDWaveData*>(Data);
			StageWaveInfo[WaveData->Stage] = *WaveData;
		}

	}
}

void UDDWaveManager::Initialize(UDDGameInstance* GameInstance)
{
	EnemySpawnManager = GameInstance->GetEnemySpawnManager();
}

void UDDWaveManager::InitStage(int32 StageNum)
{
	CurrentStage = StageNum;
	CurrentWave = 1;
	EnemyIndex = 0;
	TotalEnemyCount = 0;
	SetSplines();

	EnemySpawnManager->SetupEnemyPools(StageWaveInfo[StageNum].EnemyPoolSizes);
}

void UDDWaveManager::SetSplines()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISplineRoute::StaticClass(), OutActors);

	for (auto& Actor : OutActors)
	{
		Splines.Add(CastChecked<AAISplineRoute>(Actor));
	}
}

void UDDWaveManager::WaveStart()
{
	TotalEnemyCount = 0;

	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, [&]()
		{
			EnemySpawnManager->Activate(StageWaveInfo[CurrentStage].EnemyOrder[EnemyIndex], StageWaveInfo[CurrentStage].PathOrder[EnemyIndex]);
			EnemyIndex++;

			if (++TotalEnemyCount >= StageWaveInfo[CurrentStage].EnemyCountsPerWave[CurrentWave])
			{
				WaveEnd();
			}

		}, 3.0f, true, 0.1f);
}

void UDDWaveManager::WaveEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);

	if (++CurrentWave >= StageWaveInfo[CurrentStage].MaxWaveNum)
	{
		StageEnd();
	}
}

void UDDWaveManager::StageEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("StageEnd"));
}
