// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Trap/DDTrapStruct.h"
#include "DDTrapManager.generated.h"

UCLASS()
class PROJECTD_API UDDTrapManager : public UObject
{
	GENERATED_BODY()

public:
	UDDTrapManager();

	// 함정이 해금이 되었는지 체크하는 메서드
	UFUNCTION(BlueprintCallable)
	bool IsTowerUnlocked(const FName& TrapName) const;

	// 함정을 해금하는 메서드
	UFUNCTION(BlueprintCallable)
	void UnlockTower(const FName& TrapName);

	const FDDTrapStruct& GetTrapData(const FName& TrapName) const;

	FDDTrapStruct& GetTrapData(const FName& TrapName);

	TMap<FName, FDDTrapStruct>& GetTrapDataTable();

	const TMap<FName, FDDTrapStruct>& GetTrapDataTable() const;

	// 트랩 스폰 메서드
	UFUNCTION(BlueprintCallable)
	class ADDTrapBase* SpawnTrap(UWorld* World, const FName& TrapName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator);

private:

};
