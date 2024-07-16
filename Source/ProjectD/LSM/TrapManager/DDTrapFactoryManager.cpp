// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapManager/DDTrapFactoryManager.h"


UDDTrapFactoryManager::UDDTrapFactoryManager()
{
	
}

void UDDTrapFactoryManager::Initialize(const TMap<FName, FDDTrapStruct>& TrapDataTable)
{
	for (auto& Elem : TrapDataTable) {
		if (Elem.Value.TrapFactoryClass) {
			TrapNamesToFactories.Add(Elem.Key, Elem.Value.TrapFactoryClass);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("TrapNamesToFactories Num : %d"), TrapNamesToFactories.Num());
}

IDDTrapFactoryInterface* UDDTrapFactoryManager::GetTrapFactory(const FName& TrapName)
{
	if (UClass** FactoryClass = TrapNamesToFactories.Find(TrapName))
	{
		if (UObject** FactoryInstance = TrapFactoryInstances.Find(*FactoryClass))
		{
			return Cast<IDDTrapFactoryInterface>(*FactoryInstance);
		}
		else
		{
			// 인스턴스가 아직 생성되지 않은 경우 생성하여 저장
			UObject* NewFactoryInstance = NewObject<UObject>(this, *FactoryClass);
			TrapFactoryInstances.Add(*FactoryClass, NewFactoryInstance);
			return Cast<IDDTrapFactoryInterface>(NewFactoryInstance);
		}
	}

	return nullptr; // 팩토리를 찾을 수 없음
}
