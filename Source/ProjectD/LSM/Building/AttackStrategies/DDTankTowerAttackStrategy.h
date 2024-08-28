// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/AttackStrategies/DDProjectileAttackStrategy.h"
#include "DDTankTowerAttackStrategy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDTankTowerAttackStrategy : public UDDProjectileAttackStrategy
{
	GENERATED_BODY()

public:
	virtual void Initialize(class ADDBuildingBase* InOwningTower) override;

	virtual void Attack(AActor* TargetEnemy, const FVector& FireLocation = FVector(0, 0, 0), const FRotator& FireRotation = FRotator(0, 0, 0)) override;

private:
};
