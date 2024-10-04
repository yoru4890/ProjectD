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
	UE_LOG(LogTemp, Warning, TEXT("Pool Num :%d"), ProjectilePools[RowName].Projectiles.Num());
	if (!ProjectilePools.Contains(RowName))
	{
		ProjectilePools.Add(RowName, FProjectileList());
	}

	ADDProjectileBase* NewProjectile = nullptr;

	if (ProjectilePools[RowName].Projectiles.Num() > 0)
	{
		NewProjectile = ProjectilePools[RowName].Projectiles.Pop();
		if (NewProjectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Check Row Name :%s"), *NewProjectile->GetName());
			// Todo : 초기 설정
		}
	}
	else
	{
		check(World);
		FDDFactoryParams Params;
		Params.World = World;
		Params.RowName = RowName;
		Params.Owner = Owner;
		Params.Instigator = Instigator;
		NewProjectile = CreateProjectileInstance(Params);
	}

	NewProjectile->SetActorLocation(Location);
	NewProjectile->SetActorRotation(Rotation);
	NewProjectile->SetProjectileState(true);

	return NewProjectile;
}

ADDProjectileBase* UDDProjectileManager::CreateProjectileInstance(const FDDFactoryParams& Params)
{
	IDDFactoryInterface* ProjectileFactory = FactoryManager->GetFactory(Params.RowName);

	if (!ProjectileFactory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Find Factory"))
			return nullptr;
	}

	UObject* CreatedObject = ProjectileFactory->CreateObject(Params);
	return Cast<ADDProjectileBase>(CreatedObject);
}

void UDDProjectileManager::DestroyProjectile(ADDProjectileBase* Projectile)
{
	UE_LOG(LogTemp, Warning, TEXT("Return to Pool Call"));
	ProjectilePools[Projectile->GetRowName()].Projectiles.Add(Projectile);
	Projectile->SetActorLocation(FVector(-1000,-1000,-1000));
	Projectile->SetProjectileState(false);
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
	AssetsToLoad.Add(ProjectileData->ExplosionEffect);
	AssetsToLoad.Add(ProjectileData->ImpactSound);
	AssetsToLoad.Add(ProjectileData->FlyingSound);
}

void UDDProjectileManager::AllClearPools()
{
	// Iterate over all the Projectile Pools
	for (auto& Pool : ProjectilePools)
	{
		// Get the list of projectiles
		FProjectileList& ProjectileList = Pool.Value;

		// Destroy each projectile in the list
		for (auto& Projectile : ProjectileList.Projectiles)
		{
			if (Projectile)
			{
				Projectile->Destroy();
			}
		}

		// Clear the array of projectiles
		ProjectileList.Projectiles.Empty();
	}

	// Finally, clear the entire ProjectilePools map
	ProjectilePools.Empty();
}

void UDDProjectileManager::LoadProjectileAssets(const FName& RowName)
{
	//bool bIsAlreadyLoaded = GetProjectileData(RowName)->bIsLoaded;
	//if (bIsAlreadyLoaded)
	//{
	//	OnProjectileAssetsLoaded(RowName);
	//	return;
	//}
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
	// 로딩된 RowName에 해당하는 액터를 풀에 추가
	UE_LOG(LogTemp, Warning, TEXT("Assets for RowName %s loaded."), *RowName.ToString());

	// 풀에 해당 RowName에 대한 액터 추가
	if (!ProjectilePools.Contains(RowName))
	{
		ProjectilePools.Add(RowName, FProjectileList());
	}

	int PoolSize = GetProjectileData(RowName)->PoolSize;

	// 5개의 투사체를 생성하여 풀에 추가
	for (int32 i = 0; i < PoolSize; ++i)
	{
		FDDFactoryParams Params;
		Params.World = GetWorld();
		Params.RowName = RowName;
		Params.Owner = nullptr;
		Params.Instigator = nullptr;

		ADDProjectileBase* NewProjectile = CreateProjectileInstance(Params);
		if (NewProjectile)
		{
			ProjectilePools[RowName].Projectiles.Add(NewProjectile);
			NewProjectile->SetProjectileState(false);
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