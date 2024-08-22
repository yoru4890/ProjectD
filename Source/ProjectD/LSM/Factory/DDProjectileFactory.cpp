// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDProjectileFactory.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Projectile/DDProjectileData.h"
#include "LSM/Projectile/DDProjectileBase.h"

UObject* UDDProjectileFactory::CreateObject(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	UDDProjectileManager* ProjectileManager = MyGameInstance->GetProjectileManager();
	check(ProjectileManager);

	const FDDProjectileData& ProjectileData = *ProjectileManager->GetProjectileData(RowName);

	UClass* ProjectileClass = ProjectileData.ProjectileClass;
	check(ProjectileClass);

	check(World);
	// TrapClass를 사용하여 NewTrap 생성
	ADDProjectileBase* NewProjectile = World->SpawnActor<ADDProjectileBase>(ProjectileClass);
	check(NewProjectile);

	//NewProjectile->InitFromDataTable(RowName, *TowerData);
	//NewProjectile->SetAssets(BuildingData);
	NewProjectile->SetActorLocation(Location);
	NewProjectile->SetActorRotation(Rotation);
	NewProjectile->SetOwner(Owner);
	NewProjectile->SetInstigator(Instigator);

	return NewProjectile;
}
