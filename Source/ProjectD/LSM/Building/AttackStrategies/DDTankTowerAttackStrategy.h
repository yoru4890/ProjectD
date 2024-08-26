// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/AttackStrategies/DDBuildingBaseAttackStrategy.h"
#include "DDTankTowerAttackStrategy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDTankTowerAttackStrategy : public UDDBuildingBaseAttackStrategy
{
	GENERATED_BODY()

public:
	virtual void Initialize(class ADDBuildingBase* InOwningTower) override;

	virtual void Attack(AActor* TargetEnemy) override;

private:

	void AsyncLoadProjectileAssets(class UDDAssetManager* AssetManager, const FName& RowName);


	UPROPERTY()
	TObjectPtr<class UDDProjectileManager> ProjectileManager;

	FName ProjectileRowName;
	
};
