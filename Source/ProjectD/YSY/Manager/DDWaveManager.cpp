// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/DDWaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/AI/AISplineRoute.h"
#include "YSY/Manager/DDEnemySpawnManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Game/DDGameSingleton.h"

UDDWaveManager::UDDWaveManager()
{
}

void UDDWaveManager::Initialize()
{
	auto& TempData = UDDGameSingleton::Get().GetWaveDataTable();

	StageWaveInfo.SetNum(TempData.Num() + 1);

	for (const auto& [Name, Data] : TempData)
	{
		StageWaveInfo[Data.Stage] = Data;
	}
}

void UDDWaveManager::InitStage(int32 StageNum)
{
	CurrentStage = StageNum;
	CurrentWave = 1;
	EnemyIndex = 0;
	TotalSpawnEnemyCount = 0;
	SetSplines();
	OnSetupEnemyPoolSignature.ExecuteIfBound(StageNum);
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
	if (bIsWaveInProgress || CurrentWave >= StageWaveInfo[CurrentStage].EnemyCountsPerWave.Num())
	{
		return;
	}

	bIsWaveInProgress = true;

	TotalSpawnEnemyCount = 0;

	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, [&]()
		{
			OnActivateEnemySignature.Execute(StageWaveInfo[CurrentStage].EnemyOrder[EnemyIndex], StageWaveInfo[CurrentStage].PathOrder[EnemyIndex]);
			EnemyIndex++;

			if (++TotalSpawnEnemyCount >= StageWaveInfo[CurrentStage].EnemyCountsPerWave[CurrentWave])
			{
				SpawnEnd();
			}

		}, 3.0f, true, 0.1f);
}

void UDDWaveManager::SpawnEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("WaveSpawnEnd"));

	GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
}

void UDDWaveManager::WaveEnd()
{
	bIsWaveInProgress = false;
	UE_LOG(LogTemp, Warning, TEXT("WaveEnd"));

	if (CurrentWave++ >= StageWaveInfo[CurrentStage].EnemyCountsPerWave.Num() - 1)
	{
		StageEnd();
	}

}

void UDDWaveManager::StageEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("StageEnd"));
}

void UDDWaveManager::AddEnemyNumber()
{
	TotalEnemyCount++;
}

void UDDWaveManager::SubEnemyNumber()
{
	TotalEnemyCount--;
	if (TotalEnemyCount <= 0)
	{
		WaveEnd();
	}
}

// TODO : YSY Need Change DataInit way
void UDDWaveManager::InitEnemyNames()
{
	EnemyNames.SetNum(7);
	
	EnemyNames.Add("Error");
	EnemyNames.Add("Ironclad");
	EnemyNames.Add("Pushfire");
	EnemyNames.Add("Doomshot");
	EnemyNames.Add("Legio");
	EnemyNames.Add("ShockBomber");
	EnemyNames.Add("Obliterator");
}
