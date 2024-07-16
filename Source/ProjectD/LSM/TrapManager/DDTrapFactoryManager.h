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
	// 트랩 팩토리 인스턴스를 저장하는 맵
	TMap<UClass*, UObject*> TrapFactoryInstances;

	UPROPERTY()
	// 트랩 이름을 트랩 팩토리 인스턴스와 연관시키는 맵
	TMap<FName, UClass*> TrapNamesToFactories;
};
