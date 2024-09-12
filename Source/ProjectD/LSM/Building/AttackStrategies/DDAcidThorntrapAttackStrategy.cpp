// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDAcidThorntrapAttackStrategy.h"

void UDDAcidThorntrapAttackStrategy::Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh)
{
	ApplyDirectDamage(TargetEnemy);
	ApplyAcidDotDamge(TargetEnemy);
}
