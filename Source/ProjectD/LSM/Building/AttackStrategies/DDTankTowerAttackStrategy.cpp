// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDTankTowerAttackStrategy.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Projectile/DDProjectileBase.h"

void UDDTankTowerAttackStrategy::Initialize(ADDBuildingBase* InOwningTower)
{
	Super::Initialize(InOwningTower);
	ProjectileRowName = FName("Shell");
	InitializeProjectile(ProjectileRowName);

	ProjectileSpeed = 5000;
	ProjectileMaxSpeed = 5000;
	ProjectileLifeTime = 1;
	bIsExplosive = false;
	ExplosionRadius = 0;
	MaxPenetrationCount = 3;
}

void UDDTankTowerAttackStrategy::Attack(AActor* TargetEnemy)
{
	Super::Attack(TargetEnemy);
	Projectile->SetActorScale3D(FVector(0.3f,0.3f,0.3f));
}