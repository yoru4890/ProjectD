// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDProjectileAttackStrategy.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Projectile/DDProjectileBase.h"
#include "LSM/Building/DDBuildingBase.h"

void UDDProjectileAttackStrategy::InitializeProjectile(FName& ProjectileName)
{
    UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
    ProjectileManager = MyGameInstance->GetProjectileManager();
    check(ProjectileManager);

    // ����ü �����͸� ������
    FDDProjectileData* ProjectileData = ProjectileManager->GetProjectileData(ProjectileRowName);

    if (!ProjectileData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Strategy error"));
        return;
    }

    if (ProjectileData && !ProjectileData->bIsLoaded)
    {
        // �񵿱� �ε� �Լ� ȣ��
        ProjectileManager->LoadProjectileAssets(ProjectileRowName);
    }
}

void UDDProjectileAttackStrategy::ConfigureProjectile()
{
    if (Projectile)
    {
        Projectile->ConfigureProjectile(Damage, DamageType, ProjectileSpeed, ProjectileMaxSpeed, ProjectileLifeTime, bIsExplosive, ExplosionRadius, MaxPenetrationCount);
    }
}

void UDDProjectileAttackStrategy::Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh)
{
    Super::Attack(TargetEnemy, FireStaticMesh);
    UE_LOG(LogTemp, Warning, TEXT("Projectile Attack"));

}
