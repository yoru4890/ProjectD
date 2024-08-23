// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDGameSingleton.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"
#include "DDFactoryManager.h"
#include "LSM/Factory/DDFactoryInterface.h"
#include "LSM/Projectile/DDProjectileBase.h"

void UDDProjectileManager::Initialize()
{
}

ADDProjectileBase* UDDProjectileManager::SpawnProjectile(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	if (!ProjectilePool.Contains(RowName))
	{
		ProjectilePool.Add(RowName, FProjectileList());
	}

	ADDProjectileBase* NewProjectile = nullptr;

	if (ProjectilePool[RowName].Projectiles.Num() > 0)
	{
		NewProjectile = ProjectilePool[RowName].Projectiles.Pop();
		if (NewProjectile)
		{
			// Todo : 초기 설정
		}
	}
	else
	{
		check(World);
		NewProjectile = CreateProjectileInstance(World, RowName);
	}

	NewProjectile->SetActorHiddenInGame(false);
	NewProjectile->SetActorEnableCollision(true);
	NewProjectile->SetActorTickEnabled(true);

	return NewProjectile;
}

void UDDProjectileManager::SetupCommonReferences(UWorld* World)
{
	MyGameInstance = Cast<UDDGameInstance>(World->GetGameInstance());
	check(MyGameInstance);

	AssetManager = MyGameInstance->GetAssetManager();
	check(AssetManager);

	FactoryManager = MyGameInstance->GetFactoryManager();
	check(FactoryManager);
}

ADDProjectileBase* UDDProjectileManager::CreateProjectileInstance(UWorld* World, const FName& RowName)
{
	IDDFactoryInterface* ProjectileFactory = FactoryManager->GetFactory(RowName);
	UObject* CreatedObject = ProjectileFactory->CreateObject(World, RowName, FVector::ZeroVector, FRotator::ZeroRotator, nullptr, nullptr);
	return Cast<ADDProjectileBase>(CreatedObject);
}

void UDDProjectileManager::GetSoftObjectPtrsInProjectile(const FName& RowName, TArray<TSoftObjectPtr<UObject>>& AssetsToLoad)
{
	const FDDProjectileData* ProjectileData = GetProjectileData(RowName);

	if (!ProjectileData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Building Name"));
	}

	AssetsToLoad.Add(ProjectileData->StaticMesh);
	AssetsToLoad.Add(ProjectileData->ImpactEffect);
	AssetsToLoad.Add(ProjectileData->TrailEffect);
	AssetsToLoad.Add(ProjectileData->ImpactSound);
	AssetsToLoad.Add(ProjectileData->FlyingSound);
}


const TMap<FName, FDDProjectileData>& UDDProjectileManager::GetProjectileDataTable() const
{
	return UDDGameSingleton::Get().GetProjectileDataTable();
}

TMap<FName, FDDProjectileData>& UDDProjectileManager::GetProjectileDataTable()
{
	return UDDGameSingleton::Get().GetProjectileDataTable();
}

const FDDProjectileData* UDDProjectileManager::GetProjectileData(const FName& RowName) const
{
	const FDDProjectileData* ProjectileData = UDDGameSingleton::Get().GetProjectileDataTable().Find(RowName);

	if (ProjectileData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileData for RowName %s not found!"), *RowName.ToString());
	}

	return ProjectileData;
}

FDDProjectileData* UDDProjectileManager::GetProjectileData(const FName& RowName)
{
	FDDProjectileData* ProjectileData = UDDGameSingleton::Get().GetProjectileDataTable().Find(RowName);

	if (ProjectileData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileData for RowName %s not found!"), *RowName.ToString());
	}
	return ProjectileData;
}

void UDDProjectileManager::DestroyProjectile(ADDProjectileBase& Projectile)
{
	ProjectilePool[Projectile.GetRowName()].Projectiles.Add(Projectile);
	Projectile.SetActorHiddenInGame(true);
	Projectile.SetActorEnableCollision(false);
	Projectile.SetActorTickEnabled(false);
}