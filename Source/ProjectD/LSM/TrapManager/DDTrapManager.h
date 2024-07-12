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
	bool IsTowerUnlocked(const FName& TowerName) const;

	// ������ �ر��ϴ� �޼���
	UFUNCTION(BlueprintCallable)
	void UnlockTower(const FName& TowerName);

	FDDTrapStruct* GetTrapData(const FName& TrapName) const;

	FORCEINLINE TObjectPtr<UDataTable> GetTrapDataTable() const { return TrapDataTable; }

private:
	// �رݵ� ���� ����
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FName> UnlockedTowers;
	
	// ���� ����
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> TrapDataTable;
};
