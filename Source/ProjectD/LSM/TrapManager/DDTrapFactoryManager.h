// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Trap/DDTrapStruct.h"
#include "LSM/TrapFactory/DDTrapFactoryInterface.h"
#include "UObject/NoExportTypes.h"
#include "DDTrapFactoryManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDTrapFactoryManager : public UObject
{
	GENERATED_BODY()

public:
	UDDTrapFactoryManager();

	void Initialize(const TMap<FName, FDDTrapStruct>& TrapDataTable);

	IDDTrapFactoryInterface* GetTrapFactory(const FName& TrapName);

private:
	UPROPERTY()
	// Ʈ�� ���丮 �ν��Ͻ��� �����ϴ� ��
	TMap<UClass*, UObject*> TrapFactoryInstances;

	UPROPERTY()
	// Ʈ�� �̸��� Ʈ�� ���丮 �ν��Ͻ��� ������Ű�� ��
	TMap<FName, UClass*> TrapNamesToFactories;
};
