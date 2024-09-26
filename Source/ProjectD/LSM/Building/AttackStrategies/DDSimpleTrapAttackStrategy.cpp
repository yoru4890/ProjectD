// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDSimpleTrapAttackStrategy.h"

void UDDSimpleTrapAttackStrategy::Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh)
{
	Super::Attack(TargetEnemy, FireStaticMesh);

	ApplyDirectDamage(TargetEnemy);
	if (bIsDot)
	{
		ApplyDotDamge(TargetEnemy);
	}
	if (bIsDebuff)
	{
		ApplyDebuff(TargetEnemy);
	}
}
