// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDTankTowerAttackStrategy.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"

void UDDTankTowerAttackStrategy::Initialize(ADDBuildingBase* InOwningTower)
{
    ProjectileRowName = FName("Shell");
    UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
    ProjectileManager = MyGameInstance->GetProjectileManager();
    check(ProjectileManager);

    UDDAssetManager* AssetManager = MyGameInstance->GetAssetManager();

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

void UDDTankTowerAttackStrategy::Attack(AActor* TargetEnemy)
{
}