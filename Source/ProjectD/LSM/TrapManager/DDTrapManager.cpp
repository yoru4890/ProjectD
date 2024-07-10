// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapManager/DDTrapManager.h"

UDDTrapManager::UDDTrapManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TrapDataTableRef(TEXT("/Script/Engine.DataTable'/Game/0000/LSM/Data/LSM_DT_Trap.LSM_DT_Trap'"));
	if (TrapDataTableRef.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("TrapDataTableSuccess"));
		TrapDataTable = TrapDataTableRef.Object;
	}
	UE_LOG(LogTemp, Warning, TEXT("TrapDataTableFail"));
}


bool UDDTrapManager::IsTowerUnlocked(const FName& TowerName) const
{
	return UnlockedTowers.Contains(TowerName);
}

void UDDTrapManager::UnlockTower(const FName& TowerName)
{
	if (UnlockedTowers.Contains(TowerName))
	{
		return;
	}

	UnlockedTowers.Add(TowerName);
}

FTrapStruct* UDDTrapManager::GetTrapData(const FName& TrapName) const
{
	FTrapStruct* TrapData = nullptr;
	if (TrapDataTable)
	{
		static const FString ContextString(TEXT("Trap Data Context"));
		TrapData = TrapDataTable->FindRow<FTrapStruct>(TrapName, ContextString);
	}
	return TrapData;
}
