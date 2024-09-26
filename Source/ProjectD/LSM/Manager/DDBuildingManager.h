// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Factory/DDFactoryInterface.h"
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

UCLASS(BlueprintType)
class PROJECTD_API UDDBuildingManager : public UObject
{
	GENERATED_BODY()

public:
	UDDBuildingManager();

	void Initialize();

	// 함정이 해금이 되었는지 체크하는 메서드
	UFUNCTION(BlueprintCallable)
	bool IsBuildingUnlocked(const FName& RowName) const;

	UFUNCTION(BlueprintCallable)
	bool IsParentUnlocked(const FName& RowName);

	// 함정을 해금하는 메서드
	UFUNCTION(BlueprintCallable)
	bool UnlockBuilding(const FName& RowName);

	UFUNCTION(BlueprintCallable)
	bool LockBuilding(const FName& RowName);

	const FDDBuildingBaseData* GetBuildingData(const FName& RowName) const;

	FDDBuildingBaseData* GetBuildingData(const FName& RowName);

	const FDDTrapData* GetTrapData(const FName& RowName) const;

	const FDDTowerData* GetTowerData(const FName& RowName) const;

	const TMap<FName, FDDTrapData>& GetTrapDataTable() const;

	UFUNCTION(BlueprintCallable)
	TMap<FName, FDDTrapData>& GetTrapDataTable();

	const TMap<FName, FDDTowerData>& GetTowerDataTable() const;

	UFUNCTION(BlueprintCallable)
	TMap<FName, FDDTowerData>& GetTowerDataTable();

	const TMap<FName, FDDBuildingBaseData*>& GetBuildingDataTable() const;

	TMap<FName, FDDBuildingBaseData*>& GetBuildingDataTable();

	// 트랩 스폰 메서드
	UFUNCTION(BlueprintCallable)
	class ADDBuildingBase* SpawnBuilding(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator);

	UFUNCTION(BlueprintCallable)
	void HandleBuildingPoolsOnLevelChange();

	UFUNCTION(BlueprintCallable)
	void AllClearPools();

	void DestroyBuilding(class ADDBuildingBase& Building);

	void GetSoftObjectPtrsInBuilding(const FName& RowName, TArray<TSoftObjectPtr<UObject>>& AssetsToLoad);

	void LoadBuildingAssets(const FName& RowName);

	void UnloadBuildingAssets(const FName& RowName);

private:
	void SetupCommonReferences(UWorld* World);

	void SetBuildingSellCost(float Ratio = 0.8f);

	class ADDBuildingBase* CreateBuildingInstance(const FDDFactoryParams& Params);

	void OnBuildingAssetsLoaded(const FName& RowName);



	TMap<FName, FBuildingList> BuildingPool;

	TMap<FName, FDDBuildingBaseData*> BuildingDataTable;
	TObjectPtr<class UDDGameInstance> MyGameInstance;
	TObjectPtr<class UDDFactoryManager> FactoryManager;
	TObjectPtr<class UDDAssetManager> AssetManager;

	// Save
public:
	void SaveUnlockBuilding(const TMap<FName, bool>& BuildingUnlocked);
};
