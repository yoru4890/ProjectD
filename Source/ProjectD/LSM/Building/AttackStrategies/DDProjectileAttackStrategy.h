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
	void ConfigureProjectile();

protected:
	virtual void Attack(AActor* TargetEnemy, const FVector& FireLocation, const FRotator& FireRotation) override;

protected:
	UPROPERTY()
	TObjectPtr<class ADDProjectileBase> Projectile;
	FName ProjectileRowName;

	float ProjectileSpeed;

	float ProjectileMaxSpeed;

	float ProjectileLifeTime;

	bool bIsExplosive;

	float ExplosionRadius;

	int32 MaxPenetrationCount;

private:
	UPROPERTY()
	TObjectPtr<class UDDProjectileManager> ProjectileManager;
	
};
