// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/AttackStrategies/DDProjectileAttackStrategy.h"
#include "DDMissileTowerAttackStrategy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDMissileTowerAttackStrategy : public UDDProjectileAttackStrategy
{
	GENERATED_BODY()

public:
	virtual void Initialize(class ADDBuildingBase* InOwningTower) override;

	virtual void Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh) override;
};
