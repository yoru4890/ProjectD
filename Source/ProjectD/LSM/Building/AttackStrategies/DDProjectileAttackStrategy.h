// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/AttackStrategies/DDBaseAttackStrategy.h"
#include "DDProjectileAttackStrategy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDProjectileAttackStrategy : public UDDBaseAttackStrategy
{
	GENERATED_BODY()

protected:
	void InitializeProjectile(FName& ProjectileName);

protected:
	virtual void Attack(AActor* TargetEnemy) override;

protected:
	FName ProjectileRowName;

private:
	UPROPERTY()
	TObjectPtr<class UDDProjectileManager> ProjectileManager;
	
};
