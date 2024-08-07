// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDFactoryManager.h"
#include "LSM/Factory/DDFactoryInterface.h"

void UDDFactoryManager::Initialize(const TMap<FName, FDDBuildingBaseData*>& BuildingDataTable)
{
	for (auto& Elem : BuildingDataTable) {
		if (Elem.Value->FactoryClass) {
			NamesToFactories.Add(Elem.Key, Elem.Value->FactoryClass);
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
			// �ν��Ͻ��� ���� �������� ���� ��� �����Ͽ� ����
			UObject* NewFactoryInstance = NewObject<UObject>(this, *FactoryClass);
			FactoryInstances.Add(*FactoryClass, NewFactoryInstance);
			return Cast<IDDFactoryInterface>(NewFactoryInstance);
		}
	}

	return nullptr; // ���丮�� ã�� �� ����
}
