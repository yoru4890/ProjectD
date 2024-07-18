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

		TArray<FName> RowNames = DataTableObject->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			T* RowData = DataTableObject->FindRow<T>(RowName, "");
			if (RowData)
			{
				DataTable.Add(RowName, *RowData);
			}
			else
			{
				UE_LOG(LogDDGameSingleton, Warning, TEXT("Failed to find row: %s"), *RowName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogDDGameSingleton, Error, TEXT("Failed to load DataTable at path: %s"), *Path);
	}
}