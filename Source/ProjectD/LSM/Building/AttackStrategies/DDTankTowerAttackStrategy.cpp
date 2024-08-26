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

    // 투사체 데이터를 가져옴
    FDDProjectileData* ProjectileData = ProjectileManager->GetProjectileData(ProjectileRowName);

    if (!ProjectileData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Strategy error"));
        return;
    }

    if (ProjectileData && !ProjectileData->bIsLoaded)
    {
        // 비동기 로딩 함수 호출
        ProjectileManager->LoadProjectileAssets(ProjectileRowName);
    }


}

void UDDTankTowerAttackStrategy::Attack(AActor* TargetEnemy)
{
}