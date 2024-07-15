// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/TrapManager/DDTrapManager.h"
#include "YSY/Game/DDGameSingleton.h"
#include "YSY/Game/DDGameInstance.h"
#include "GameFramework/Actor.h"

UDDTrapManager::UDDTrapManager()
{

}

void UDDTrapManager::SetTrapFactoryManager(UDDTrapFactoryManager* InTrapFactoryManager)
{
	TrapFactoryManager = InTrapFactoryManager;
}


bool UDDTrapManager::IsTowerUnlocked(const FName& TrapName) const
{
	const FDDTrapStruct* TrapStruct = GetTrapData(TrapName);
	check(TrapStruct);

	return TrapStruct->bIsTrapUnlocked;

}

void UDDTrapManager::UnlockTower(const FName& TrapName)
{
	FDDTrapStruct* TrapStruct = GetTrapData(TrapName);
	TrapStruct->bIsTrapUnlocked = true;

}

const FDDTrapStruct* UDDTrapManager::GetTrapData(const FName& TrapName) const
{
	return UDDGameSingleton::Get().GetTrapDataTable().Find(TrapName);
}

FDDTrapStruct* UDDTrapManager::GetTrapData(const FName& TrapName)
{
	return UDDGameSingleton::Get().GetTrapDataTable().Find(TrapName);
}

TMap<FName, FDDTrapStruct>& UDDTrapManager::GetTrapDataTable()
{
	return UDDGameSingleton::Get().GetTrapDataTable();
}

const TMap<FName, FDDTrapStruct>& UDDTrapManager::GetTrapDataTable() const
{
	return UDDGameSingleton::Get().GetTrapDataTable();
}

ADDTrapBase* UDDTrapManager::SpawnTrap(const UWorld* World, const FName& TrapName, const FVector& Location, const FRotator& Rotation)
{
	check(World); // World가 null이면 실행 중지

	const FDDTrapStruct* TrapStruct = GetTrapData(TrapName);
	check(TrapStruct); // TrapStruct가 null이면 실행 중지

	TObjectPtr<UDDGameInstance> MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);



	return nullptr;

}
