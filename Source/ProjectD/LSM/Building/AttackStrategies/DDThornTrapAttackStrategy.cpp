// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDThornTrapAttackStrategy.h"

void UDDThornTrapAttackStrategy::Attack(AActor* TargetEnemy, const FVector& FireLocation, const FRotator& FireRotation)
{
	Super::Attack(TargetEnemy, FireLocation, FireRotation);

	ApplyDirectDamage(TargetEnemy);
}
