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
	// 트랩 팩토리 인스턴스를 저장하는 맵
	TMap<TSubclassOf<UObject>, UObject*> TrapFactoryInstances;

	UPROPERTY()
	// 트랩 이름을 트랩 팩토리 인스턴스와 연관시키는 맵
	TMap<FName, TSubclassOf<UObject>> TrapNamesToFactories;

	UPROPERTY()
	// TrapManager 참조
	class UDDTrapManager* TrapManager;
};
