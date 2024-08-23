// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDFactoryManager.h"
#include "LSM/Factory/DDFactoryInterface.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "YSY/Game/DDGameSingleton.h"

void UDDFactoryManager::Initialize()
{
	TMap<FName,FDDTrapData>& TrapDataTable = UDDGameSingleton::Get().GetTrapDataTable();
	TMap<FName,FDDTowerData>& TowerDataTable = UDDGameSingleton::Get().GetTowerDataTable();
	TMap<FName,FDDProjectileData>& ProjectileDataTable = UDDGameSingleton::Get().GetProjectileDataTable();
	for (auto& Elem : TrapDataTable) {
		if (Elem.Value.FactoryClass) {
			NamesToFactories.Add(Elem.Key, Elem.Value.FactoryClass);
		}
	}

	for (auto& Elem : TowerDataTable)
	{
		if (Elem.Value.FactoryClass)
		{
			NamesToFactories.Add(Elem.Key, Elem.Value.FactoryClass);
		}
	}

	for (auto& Elem : ProjectileDataTable)
	{
		if (Elem.Value.FactoryClass)
		{
			NamesToFactories.Add(Elem.Key, Elem.Value.FactoryClass);
		}
	}

	

	UE_LOG(LogTemp, Warning, TEXT("TrapNamesToFactories Num : %d"), NamesToFactories.Num());
}

IDDFactoryInterface* UDDFactoryManager::GetFactory(const FName& RowName)
{
	if (UClass** FactoryClass = NamesToFactories.Find(RowName))
	{
		if (UObject** FactoryInstance = FactoryInstances.Find(*FactoryClass))
		{
			return Cast<IDDFactoryInterface>(*FactoryInstance);
		}
		else
		{
			// 인스턴스가 아직 생성되지 않은 경우 생성하여 저장
			UObject* NewFactoryInstance = NewObject<UObject>(this, *FactoryClass);
			FactoryInstances.Add(*FactoryClass, NewFactoryInstance);
			return Cast<IDDFactoryInterface>(NewFactoryInstance);
		}
	}

	return nullptr; // 팩토리를 찾을 수 없음
}
