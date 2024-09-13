// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDTankTowerAttackStrategy.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "LSM/Projectile/DDProjectileBase.h"

void UDDTankTowerAttackStrategy::Initialize(ADDBuildingBase* InOwningTower)
{
	Super::Initialize(InOwningTower);
	ProjectileRowName = FName("Shell");
	InitializeProjectile(ProjectileRowName);

	ProjectileSpeed = 5000;
	ProjectileMaxSpeed = 5000;
	ProjectileLifeTime = 5;
	bIsExplosive = false;
	ExplosionRadius = 0;
	MaxPenetrationCount = 3;
}

void UDDTankTowerAttackStrategy::Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh)
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
			if (!Projectile)
			{
				UE_LOG(LogTemp, Warning, TEXT("Projectile is null"));
				return;
			}
			ConfigureProjectile();
			Projectile->SetActorScale3D(FVector(0.3f, 0.3f, 0.3f));
		}
	}
}