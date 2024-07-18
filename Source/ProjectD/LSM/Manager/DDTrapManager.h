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

	// ������ �ر��� �Ǿ����� üũ�ϴ� �޼���
	UFUNCTION(BlueprintCallable)
	bool IsTowerUnlocked(const FName& TrapName) const;

	// ������ �ر��ϴ� �޼���
	UFUNCTION(BlueprintCallable)
	void UnlockTower(const FName& TrapName);

	const FDDTrapStruct& GetTrapData(const FName& TrapName) const;

	FDDTrapStruct& GetTrapData(const FName& TrapName);

	TMap<FName, FDDTrapStruct>& GetTrapDataTable();

	const TMap<FName, FDDTrapStruct>& GetTrapDataTable() const;

	// Ʈ�� ���� �޼���
	UFUNCTION(BlueprintCallable)
	class ADDTrapBase* SpawnTrap(UWorld* World, const FName& TrapName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator);

private:

};
