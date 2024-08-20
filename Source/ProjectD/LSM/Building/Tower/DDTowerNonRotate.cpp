// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerNonRotate.h"

void ADDTowerNonRotate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastAttack += DeltaTime;

	if (bCanAttack && !EnemiesInRanged.IsEmpty() && TimeSinceLastAttack >= AttackCoolTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		ExecuteAttackEffects();
		TimeSinceLastAttack = 0.f;
	}
}
