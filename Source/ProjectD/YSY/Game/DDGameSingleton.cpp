// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameSingleton.h"

DEFINE_LOG_CATEGORY(LogDDGameSingleton);

UDDGameSingleton::UDDGameSingleton()
{
	LoadDataTable<FDDEnemyData>(EnemyDataTable, TEXT("/Script/Engine.DataTable'/Game/0000/YSY/Data/DT_EnemyData.DT_EnemyData'"));
	LoadDataTable<FDDTrapStruct>(TrapDataTable, TEXT("/Script/Engine.DataTable'/Game/0000/LSM/Data/LSM_DT_Trap.LSM_DT_Trap'"));
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


template<typename T>
void UDDGameSingleton::LoadDataTable(TMap<FName, T>& DataTable, const FString& Path)
{
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(*Path);
	if (DataTableRef.Object)
	{
		const UDataTable* DataTableObject = DataTableRef.Object;
		check(DataTableObject->GetRowMap().Num() > 0);

		TMap<FName, uint8*> TempDataMap = DataTableObject->GetRowMap();
		for (auto& Elem : TempDataMap)
		{
			DataTable.Add(Elem.Key, *reinterpret_cast<T*>(Elem.Value));
		}
	}
	else
	{
		UE_LOG(LogDDGameSingleton, Error, TEXT("Failed to load DataTable at path: %s"), *Path);
	}
}