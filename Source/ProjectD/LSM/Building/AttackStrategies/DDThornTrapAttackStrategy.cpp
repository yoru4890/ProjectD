// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDThornTrapAttackStrategy.h"

void UDDThornTrapAttackStrategy::Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh)
{
	Super::Attack(TargetEnemy, FireStaticMesh);

	ApplyDirectDamage(TargetEnemy);
}
