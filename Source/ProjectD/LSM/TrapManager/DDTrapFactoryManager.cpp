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
			// �ν��Ͻ��� ���� �������� ���� ��� �����Ͽ� ����
			UObject* NewFactoryInstance = NewObject<UObject>(this, *FactoryClass);
			TrapFactoryInstances.Add(*FactoryClass, NewFactoryInstance);
			return Cast<IDDTrapFactoryInterface>(NewFactoryInstance);
		}
	}

	return nullptr; // ���丮�� ã�� �� ����
}

void UDDTrapFactoryManager::SetTrapManager(UDDTrapManager* InTrapManager)
{
	TrapManager = InTrapManager;
}
