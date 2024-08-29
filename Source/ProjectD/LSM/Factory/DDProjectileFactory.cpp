// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDProjectileFactory.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Projectile/DDProjectileData.h"
#include "LSM/Projectile/DDProjectileBase.h"

UObject* UDDProjectileFactory::CreateObject(const FDDFactoryParams& Params)
{
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(Params.World->GetGameInstance());
	check(MyGameInstance);

	UDDProjectileManager* ProjectileManager = MyGameInstance->GetProjectileManager();
	check(ProjectileManager);

	const FDDProjectileData& ProjectileData = *ProjectileManager->GetProjectileData(Params.RowName);

	UClass* ProjectileClass = ProjectileData.ProjectileClass;
	check(ProjectileClass);

	// TrapClass를 사용하여 NewTrap 생성
	ADDProjectileBase* NewProjectile = Params.World->SpawnActor<ADDProjectileBase>(ProjectileClass);
	check(NewProjectile);

	//NewProjectile->InitFromDataTable(RowName, *TowerData);
	NewProjectile->SetRowName(Params.RowName);
	NewProjectile->SetAssetAndManager(ProjectileData,ProjectileManager);
	NewProjectile->SetProjectileState(false);
	NewProjectile->SetActorLocation(FVector(-1000, 0, 0));
	NewProjectile->SetOwner(Params.Owner);
	NewProjectile->SetInstigator(Params.Instigator);

	return NewProjectile;
}
