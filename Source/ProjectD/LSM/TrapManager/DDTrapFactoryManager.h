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

	void Initialize(TMap<FName, FDDTrapStruct>& TrapDataTable);

	const IDDTrapFactoryInterface* GetTrapFactory(const FName& TrapName);

	void SetTrapManager(class UDDTrapManager* InTrapManager);

private:
	UPROPERTY()
	// Ʈ�� ���丮 �ν��Ͻ��� �����ϴ� ��
	TMap<TSubclassOf<UObject>, UObject*> TrapFactoryInstances;

	UPROPERTY()
	// Ʈ�� �̸��� Ʈ�� ���丮 �ν��Ͻ��� ������Ű�� ��
	TMap<FName, TSubclassOf<UObject>> TrapNamesToFactories;

	UPROPERTY()
	// TrapManager ����
	class UDDTrapManager* TrapManager;
};
