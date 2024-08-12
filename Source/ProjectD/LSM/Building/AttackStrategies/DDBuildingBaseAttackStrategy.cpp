// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDBuildingBaseAttackStrategy.h"
#include "Engine/DamageEvents.h"
#include "LSM/Building/DDBuildingBase.h"


void UDDBuildingBaseAttackStrategy::Initialize(ADDBuildingBase* InOwningTower)
{
	OwningTower = InOwningTower;
	DamageType = InOwningTower->GetDamageType();
	Damage = InOwningTower->GetDamage();
	DotDamage = InOwningTower->GetDamage();
	DotDuration = InOwningTower->GetDamage();
	DotInterval = InOwningTower->GetDamage();
	SlowAmount = InOwningTower->GetDamage();
	SlowDuration = InOwningTower->GetDamage();
}


void UDDBuildingBaseAttackStrategy::Attack(AActor* TargetEnemy)
{

}

void UDDBuildingBaseAttackStrategy::ApplyDotDamge(AActor* TargetEnemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Apply Dot Damage"));
}

void UDDBuildingBaseAttackStrategy::ApplyDirectDamage(AActor* TargetEnemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Apply Direct Damage"));
	FDamageEvent DamageEvent{};
	DamageEvent.DamageTypeClass = DamageType;
	TargetEnemy->TakeDamage(Damage, DamageEvent, nullptr, OwningTower);
}

void UDDBuildingBaseAttackStrategy::ApplySlowEffect(AActor* TargetEnemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Apply Slow Effect"));
}
