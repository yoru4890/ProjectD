// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDMissileTowerAttackStrategy.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "LSM/Projectile/DDMissileProjectile.h"

void UDDMissileTowerAttackStrategy::Initialize(ADDBuildingBase* InOwningTower)
{
	Super::Initialize(InOwningTower);
	ProjectileRowName = FName("Missile");
	InitializeProjectile(ProjectileRowName);

	ProjectileSpeed = 300;
	ProjectileMaxSpeed = 30000;
	ProjectileLifeTime = 10;
	bIsExplosive = true;
	ExplosionRadius = 250;
	MaxPenetrationCount = 1;
}

void UDDMissileTowerAttackStrategy::Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh)
{
	Super::Attack(TargetEnemy, FireStaticMesh);

	const TArray<FName>& SocketNames = FireStaticMesh->GetAllSocketNames();
	for (const FName& SocketName : SocketNames)
	{
		if (SocketName.ToString().StartsWith(TEXT("FirePoint")))
		{
			FVector FirePointLocation = FireStaticMesh->GetSocketLocation(SocketName);
			FRotator FirePointDirection = FireStaticMesh->GetSocketRotation(SocketName);
			Projectile = ProjectileManager->SpawnProjectile(GetWorld(), ProjectileRowName, FirePointLocation, FirePointDirection, nullptr, nullptr);
			ADDMissileProjectile*  MissileProjectile = Cast<ADDMissileProjectile>(Projectile);
			if (MissileProjectile)
			{
				MissileProjectile->SetTargetActor(TargetEnemy);
			}
			ConfigureProjectile();

			UE_LOG(LogTemp, Warning, TEXT("Strategy Fire"));
		}
	}
}
