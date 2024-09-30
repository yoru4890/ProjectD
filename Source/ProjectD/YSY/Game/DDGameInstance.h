// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "DDGameInstance.generated.h"

/**
 *
 */
UCLASS()
class PROJECTD_API UDDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	FORCEINLINE class UDDAssetManager* GetAssetManager() const { return AssetManager; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UDDBuildingManager* GetBuildingManager() const { return BuildingManager; }

	FORCEINLINE class UDDFactoryManager* GetFactoryManager() const { return FactoryManager; }

	FORCEINLINE FStreamableManager& GetStreamableManager() { return StreamableManager; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UDDEnemySpawnManager* GetEnemySpawnManager() const { return EnemySpawnManager; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UDDWaveManager* GetWaveManager() const { return WaveManager; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UDDProjectileManager* GetProjectileManager() const { return ProjectileManager; }

	FORCEINLINE class UDDDataManager* GetDataManager() const { return DataManager; }

	void InitializeManagerDelegates();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDAssetManager> AssetManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDBuildingManager> BuildingManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDFactoryManager> FactoryManager;

	FStreamableManager StreamableManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDEnemySpawnManager> EnemySpawnManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDWaveManager> WaveManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDProjectileManager> ProjectileManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDDataManager> DataManager;
};
