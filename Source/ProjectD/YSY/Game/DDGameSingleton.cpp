// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameSingleton.h"

DEFINE_LOG_CATEGORY(LogDDGameSingleton);

UDDGameSingleton::UDDGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyDataTableRef(TEXT("/Script/Engine.DataTable'/Game/0000/YSY/Data/DT_EnemyData.DT_EnemyData'"));
	if (EnemyDataTableRef.Object)
	{
		const UDataTable* DataTable = EnemyDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TMap<FName, uint8*> TempEnemyDataMap = DataTable->GetRowMap();
		for (auto& [EnemyName, EnemyData] : TempEnemyDataMap)
		{
			EnemyDataTable.Add(EnemyName, *reinterpret_cast<FDDEnemyData*>(EnemyData));
		}
	}
}

UDDGameSingleton& UDDGameSingleton::Get()
{
	UDDGameSingleton* Singleton = CastChecked<UDDGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogDDGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UDDGameSingleton>();
}

FDDEnemyData UDDGameSingleton::GetEnemyData(const FName& EnemyName) const
{
	const FDDEnemyData* EnemyData = EnemyDataTable.Find(EnemyName);
	if (EnemyData)
	{
		return *EnemyData;
	}

	return FDDEnemyData();
}
