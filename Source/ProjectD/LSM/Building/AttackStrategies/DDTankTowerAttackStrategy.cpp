// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDTankTowerAttackStrategy.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"

void UDDTankTowerAttackStrategy::Initialize(ADDBuildingBase* InOwningTower)
{
	ProjectileRowName = FName("Shell");
	InitializeProjectile(ProjectileRowName);
	Super::Initialize(InOwningTower);

}

void UDDTankTowerAttackStrategy::Attack(AActor* TargetEnemy)
{
}