// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDMachineGunTowerAttackStrategy.h"
#include "LSM/Building/DDBuildingBase.h"
#include "Engine/DamageEvents.h"

void UDDMachineGunTowerAttackStrategy::Attack(AActor* TargetEnemy)
{
	Super::Attack(TargetEnemy);

	ApplyDirectDamage(TargetEnemy);
	PlayHitEffect(TargetEnemy);
}
