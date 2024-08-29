// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Factory/DDFactoryInterface.h"
#include "LSM/Projectile/DDProjectileData.h"
#include "DDProjectileManager.generated.h"

USTRUCT(BlueprintType)
struct FProjectileList
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class ADDProjectileBase>> Projectiles;
};

UCLASS()
class PROJECTD_API UDDProjectileManager : public UObject
{
	GENERATED_BODY()
	

public:
	void Initialize();

	class ADDProjectileBase* SpawnProjectile(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator);

	void DestroyProjectile(class ADDProjectileBase* Projectile);

	void GetSoftObjectPtrsInProjectile(const FName& RowName, TArray<TSoftObjectPtr<UObject>>& AssetsToLoad);


	const TMap<FName, FDDProjectileData>& GetProjectileDataTable() const;

	TMap<FName, FDDProjectileData>& GetProjectileDataTable();

	const FDDProjectileData* GetProjectileData(const FName& RowName) const;

	FDDProjectileData* GetProjectileData(const FName& RowName);

	void LoadProjectileAssets(const FName& RowName);



private:
	void SetupCommonReferences(UWorld* World);

	void OnProjectileAssetsLoaded(const FName& RowName);

	class ADDProjectileBase* CreateProjectileInstance(const FDDFactoryParams& Params);
	TMap<FName, FProjectileList> ProjectilePool;
	TObjectPtr<class UDDGameInstance> MyGameInstance;
	TObjectPtr<class UDDFactoryManager> FactoryManager;
	TObjectPtr<class UDDAssetManager> AssetManager;
};
