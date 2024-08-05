// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Trap/DDTrapData.h"
#include "DDTrapManager.generated.h"

USTRUCT(BlueprintType)
struct FTrapList
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<ADDTrapBase>> Traps;
};

UCLASS()
class PROJECTD_API UDDTrapManager : public UObject
{
	GENERATED_BODY()

public:
	UDDTrapManager();

	// ������ �ر��� �Ǿ����� üũ�ϴ� �޼���
	UFUNCTION(BlueprintCallable)
	bool IsTrapUnlocked(const FName& RowName) const;

	// ������ �ر��ϴ� �޼���
	UFUNCTION(BlueprintCallable)
	bool UnlockTrap(const FName& RowName);

	bool LockTrap(const FName& RowName);

	const FDDTrapData& GetTrapData(const FName& RowName) const;

	FDDTrapData& GetTrapData(const FName& RowName);

	TMap<FName, FDDTrapData>& GetTrapDataTable();

	const TMap<FName, FDDTrapData>& GetTrapDataTable() const;

	// Ʈ�� ���� �޼���
	UFUNCTION(BlueprintCallable)
	class ADDTrapBase* SpawnTrap(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator);

	void DestroyTrap(class ADDTrapBase& Trap);

private:
	TMap<FName, FTrapList> TrapPool;

};
