// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Building/Tower/DDTowerData.h"
#include "LSM/Building/Trap/DDTrapData.h"
#include "DDBuildingManager.generated.h"

USTRUCT(BlueprintType)
struct FBuildingList
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<ADDBuildingBase>> Buildings;
};

UCLASS()
class PROJECTD_API UDDBuildingManager : public UObject
{
	GENERATED_BODY()

public:
	UDDBuildingManager();

	void Initialize();

	// ������ �ر��� �Ǿ����� üũ�ϴ� �޼���
	UFUNCTION(BlueprintCallable)
	bool IsBuildingUnlocked(const FName& RowName) const;

	// ������ �ر��ϴ� �޼���
	UFUNCTION(BlueprintCallable)
	bool UnlockBuilding(const FName& RowName);

	bool LockBuilding(const FName& RowName);

	const FDDBuildingBaseData* GetBuildingData(const FName& RowName) const;

	FDDBuildingBaseData* GetBuildingData(const FName& RowName);

	const FDDTrapData* GetTrapData(const FName& RowName) const;

	const FDDTowerData* GetTowerData(const FName& RowName) const;

	const TMap<FName, FDDTrapData>& GetTrapDataTable() const;

	TMap<FName, FDDTrapData>& GetTrapDataTable();

	const TMap<FName, FDDTowerData>& GetTowerDataTable() const;

	TMap<FName, FDDTowerData>& GetTowerDataTable();

	const TMap<FName, FDDBuildingBaseData*>& GetBuildingDataTable() const;

	TMap<FName, FDDBuildingBaseData*>& GetBuildingDataTable();

	// Ʈ�� ���� �޼���
	UFUNCTION(BlueprintCallable)
	class ADDBuildingBase* SpawnBuilding(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator);

	void DestroyBuilding(class ADDBuildingBase& Building);

private:
	TMap<FName, FBuildingList> BuildingPool;

	TMap<FName, FDDBuildingBaseData*> BuildingDataTable;

};
