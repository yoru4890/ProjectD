// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDMachineGunTowerAttackStrategy.h"
#include "LSM/Building/DDBuildingBase.h"
#include "Engine/DamageEvents.h"

void UDDMachineGunTowerAttackStrategy::Attack(AActor* TargetEnemy, const FVector& FireLocation, const FRotator& FireRotation)
{
	Super::Attack(TargetEnemy, FireLocation, FireRotation);

	ApplyDirectDamage(TargetEnemy);
	PlayHitEffect(TargetEnemy);
}
