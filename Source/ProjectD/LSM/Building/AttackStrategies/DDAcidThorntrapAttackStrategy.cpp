// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDAcidThorntrapAttackStrategy.h"

void UDDAcidThorntrapAttackStrategy::Attack(AActor* TargetEnemy, const FVector& FireLocation, const FRotator& FireRotation)
{
	ApplyDirectDamage(TargetEnemy);
	ApplyAcidDotDamge(TargetEnemy);
}
