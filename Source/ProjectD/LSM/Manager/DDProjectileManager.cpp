// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Game/DDDataManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"
#include "DDFactoryManager.h"
#include "LSM/Factory/DDFactoryInterface.h"
#include "LSM/Projectile/DDProjectileBase.h"

#pragma region Initalize
void UDDProjectileManager::Initialize()
{
	SetupCommonReferences(GetWorld());
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

#pragma endregion Initalize

#pragma region SpawnAndDestroy

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
			// Todo : �ʱ� ����
		}
	}
	else
	{
		check(World);
		NewProjectile = CreateProjectileInstance(World, RowName);
	}

	NewProjectile->SetProjectileActive(true);

	return NewProjectile;
}

ADDProjectileBase* UDDProjectileManager::CreateProjectileInstance(UWorld* World, const FName& RowName)
{
	IDDFactoryInterface* ProjectileFactory = FactoryManager->GetFactory(RowName);
	UObject* CreatedObject = ProjectileFactory->CreateObject(World, RowName, FVector::ZeroVector, FRotator::ZeroRotator, nullptr, nullptr);
	return Cast<ADDProjectileBase>(CreatedObject);
}

void UDDProjectileManager::DestroyProjectile(ADDProjectileBase* Projectile)
{
	ProjectilePool[Projectile->GetRowName()].Projectiles.Add(Projectile);
	Projectile->SetProjectileActive(false);
}

#pragma endregion SpawnAndDestroy

#pragma region Asset

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

void UDDProjectileManager::LoadProjectileAssets(const FName& RowName)
{
	bool bIsAlreadyLoaded = GetProjectileData(RowName)->bIsLoaded;
	if (bIsAlreadyLoaded)
	{
		return;
	}
	TArray<TSoftObjectPtr<UObject>> AssetsToLoad;

	GetSoftObjectPtrsInProjectile(RowName, AssetsToLoad);

	GetProjectileData(RowName)->bIsLoaded = true;

	AssetManager->LoadAssetsAsync(AssetsToLoad, FStreamableDelegate::CreateLambda([this, RowName]()
		{
			OnProjectileAssetsLoaded(RowName);
		}));

}

void UDDProjectileManager::OnProjectileAssetsLoaded(const FName& RowName)
{
	// �ε��� RowName�� �ش��ϴ� ���͸� Ǯ�� �߰�
	UE_LOG(LogTemp, Warning, TEXT("Assets for RowName %s loaded."), *RowName.ToString());

	// Ǯ�� �ش� RowName�� ���� ���� �߰�
	if (!ProjectilePool.Contains(RowName))
	{
		ProjectilePool.Add(RowName, FProjectileList());
	}

	int PoolSize = GetProjectileData(RowName)->PoolSize;

	// 5���� ����ü�� �����Ͽ� Ǯ�� �߰�
	for (int32 i = 0; i < PoolSize; ++i)
	{
		ADDProjectileBase* NewProjectile = CreateProjectileInstance(GetWorld(), RowName);
		if (NewProjectile)
		{
			ProjectilePool[RowName].Projectiles.Add(NewProjectile);
			UE_LOG(LogTemp, Warning, TEXT("Projectile %d for RowName %s added to pool."), i + 1, *RowName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create projectile %d for RowName %s."), i + 1, *RowName.ToString());
		}
	}
}

#pragma endregion Asset

#pragma region Data

const TMap<FName, FDDProjectileData>& UDDProjectileManager::GetProjectileDataTable() const
{
	return MyGameInstance->GetDataManager()->GetProjectileDataTable();
}

TMap<FName, FDDProjectileData>& UDDProjectileManager::GetProjectileDataTable()
{
	return MyGameInstance->GetDataManager()->GetProjectileDataTable();
}

const FDDProjectileData* UDDProjectileManager::GetProjectileData(const FName& RowName) const
{
	const FDDProjectileData* ProjectileData = MyGameInstance->GetDataManager()->GetProjectileDataTable().Find(RowName);

	if (!ProjectileData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileData for RowName %s not found!"), *RowName.ToString());
		return nullptr;
	}

	return ProjectileData;
}

FDDProjectileData* UDDProjectileManager::GetProjectileData(const FName& RowName)
{
	check(MyGameInstance);
	check(MyGameInstance->GetDataManager());
	FDDProjectileData* ProjectileData = MyGameInstance->GetDataManager()->GetProjectileDataTable().Find(RowName);
	check(ProjectileData);

	if (!ProjectileData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileData for RowName %s not found!"), *RowName.ToString());
		return nullptr;
	}
	return ProjectileData;
}

#pragma endregion Data