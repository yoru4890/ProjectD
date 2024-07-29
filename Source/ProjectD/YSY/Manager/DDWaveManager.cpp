// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/DDWaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/AI/AISplineRoute.h"

UDDWaveManager::UDDWaveManager()
{
	ConstructorHelpers::FObjectFinder<UDataTable> WaveDataTableRef(TEXT("/Script/Engine.DataTable'/Game/0000/YSY/Data/YSY_DT_WaveData.YSY_DT_WaveData'"));

	if (WaveDataTableRef.Object)
	{
		const UDataTable* DataTableObject = WaveDataTableRef.Object;
		check(DataTableObject->GetRowMap().Num() > 0);

		WaveInfo.SetNum(DataTableObject->GetRowMap().Num() + 1);

		const int32 MaxWave{ 10 };

		for (auto& Wave : WaveInfo)
		{
			Wave.SetNum(MaxWave + 1);
		}

		auto& DataMap = DataTableObject->GetRowMap();
		for (auto& [RowName, Data] : DataMap)
		{
			auto WaveData = reinterpret_cast<FDDWaveData*>(Data);
			WaveInfo[WaveData->Stage][WaveData->WaveNumber] = MoveTemp(WaveData);
		}

	}
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
