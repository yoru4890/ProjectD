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
	// 트랩 팩토리 인스턴스를 저장하는 맵
	TMap<UClass*, UObject*> FactoryInstances;

	UPROPERTY()
	// 트랩 이름을 트랩 팩토리 인스턴스와 연관시키는 맵
	TMap<FName, UClass*> NamesToFactories;
};
