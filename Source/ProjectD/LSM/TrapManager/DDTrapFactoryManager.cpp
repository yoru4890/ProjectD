// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapManager/DDTrapFactoryManager.h"
#include "LSM/TrapFactory/DDAnimatedTrapFactory.h"


UDDTrapFactoryManager::UDDTrapFactoryManager()
{
	TrapNamesToFactories.Add("ThornTrap", UDDAnimatedTrapFactory::StaticClass());
}

void UDDTrapFactoryManager::Initialize(TMap<FName, FDDTrapStruct>& TrapDataTable)
{

}

const IDDTrapFactoryInterface* UDDTrapFactoryManager::GetTrapFactory(const FName& TrapName)
{
	if (const TSubclassOf<UObject>* FactoryClass = TrapNamesToFactories.Find(TrapName))
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

void UDDTrapFactoryManager::SetTrapManager(UDDTrapManager* InTrapManager)
{
	TrapManager = InTrapManager;
}
