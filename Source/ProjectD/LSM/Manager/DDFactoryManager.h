// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Trap/DDTrapStruct.h"
#include "DDFactoryManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDFactoryManager : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(const TMap<FName, FDDTrapStruct>& TrapDataTable /* other DataTable can Add*/);

	class IDDFactoryInterface* GetFactory(const FName& RowName);

private:
	UPROPERTY()
	// Ʈ�� ���丮 �ν��Ͻ��� �����ϴ� ��
	TMap<UClass*, UObject*> FactoryInstances;

	UPROPERTY()
	// Ʈ�� �̸��� Ʈ�� ���丮 �ν��Ͻ��� ������Ű�� ��
	TMap<FName, UClass*> NamesToFactories;
};
