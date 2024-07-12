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
	bool IsTowerUnlocked(const FName& TowerName) const;

	// 함정을 해금하는 메서드
	UFUNCTION(BlueprintCallable)
	void UnlockTower(const FName& TowerName);

	FDDTrapStruct* GetTrapData(const FName& TrapName) const;

	FORCEINLINE TObjectPtr<UDataTable> GetTrapDataTable() const { return TrapDataTable; }

private:
	// 해금된 함정 집합
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FName> UnlockedTowers;
	
	// 함정 정보
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> TrapDataTable;
};
