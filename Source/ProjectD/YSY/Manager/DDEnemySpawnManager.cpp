// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/DDEnemySpawnManager.h"
#include "YSY/Enemy/DDEnemyBase.h"
#include "YSY/Game/DDDataManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Manager/DDWaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UDDEnemySpawnManager::UDDEnemySpawnManager()
{
}

void UDDEnemySpawnManager::SetupEnemyPools(int32 StageNum)
{
	FString StringNum = FString::Printf(TEXT("%d"), StageNum);
	FName StageName(*StringNum);
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	const TMap<FName, int32>& EnemyPoolSizes = MyGameInstance->GetDataManager()->GetWaveDataTable().Find(StageName)->EnemyPoolSizes;

	for (auto& [EnemyName, PoolSize] : EnemyPoolSizes)
	{
		ActiveObjects.FindOrAdd(EnemyName, TArray<ADDEnemyBase*>());
		InactiveObjects.FindOrAdd(EnemyName, TArray<ADDEnemyBase*>());

		for (int32 i{}; i < PoolSize; i++)
		{
			SpawnEnemy(EnemyName);
		}
	}

}

AActor* UDDEnemySpawnManager::Activate(const FName& EnemyName, int32 SplineIndex)
{
	if (InactiveObjects.Contains(EnemyName) && InactiveObjects[EnemyName].Num() > 0)
	{
		ADDEnemyBase* Enemy = InactiveObjects[EnemyName].Last();
		InactiveObjects[EnemyName].Pop();
		ActiveObjects[EnemyName].Add(Enemy);
		UDDGameInstance* DDGameInstance = Cast<UDDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		Enemy->SetAIMoveRoute(DDGameInstance->GetWaveManager()->GetSplines(), SplineIndex);
		Enemy->Activate();
		OnAddEnemySignature.ExecuteIfBound();
		return Enemy;
	}
	else
	{
		// TODO : YSY No Enemy in Pools. Need to Add Enemy
		return nullptr;
	}

	return nullptr;
}

void UDDEnemySpawnManager::Deactivate(const FName& EnemyName, ADDEnemyBase* Enemy)
{
	if (ActiveObjects.Contains(EnemyName) && ActiveObjects[EnemyName].Remove(Enemy))
	{
		InactiveObjects[EnemyName].Add(Enemy);
		Enemy->Deactivate();
		OnSubEnemySignature.ExecuteIfBound();
	}
}

void UDDEnemySpawnManager::SpawnEnemy(const FName& EnemyName)
{
	// TODO : YSY Setting SpawnTransform

	ADDEnemyBase* Enemy = GetWorld()->SpawnActorDeferred<ADDEnemyBase>(ADDEnemyBase::StaticClass(), {});

	// TODO : YSY Casting X
	UDDGameInstance* DDGameInstance = Cast<UDDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto TempWaveManager = DDGameInstance->GetWaveManager();

	if (Enemy)
	{
		UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
		Enemy->OnDie.AddUObject(this, &UDDEnemySpawnManager::Deactivate);
		Enemy->OnSubRemainingLivesSignature.BindUObject(TempWaveManager, &UDDWaveManager::SubtractRemainingLives);
		Enemy->InitializeEnemy(*MyGameInstance->GetDataManager()->GetEnemyDataTable().Find(EnemyName));
		Enemy->FinishSpawning({});
		Enemy->Deactivate();
		InactiveObjects[EnemyName].Add(Enemy);
	}
}

void UDDEnemySpawnManager::ClearEnemyPool()
{
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	auto& EnemyDataTable = MyGameInstance->GetDataManager()->GetEnemyDataTable();

	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();

	TArray<FName> AllEnemyNames;

	ActiveObjects.GetKeys(AllEnemyNames);

	for (const auto& EnemyName : AllEnemyNames)
	{
		const auto& EnemyData = EnemyDataTable.Find(EnemyName);

		if (EnemyData->SkeletalMesh.IsValid())
		{
			AssetLoader.Unload(EnemyData->SkeletalMesh.ToSoftObjectPath());
		}

		/*if (EnemyData->AnimationBlueprint.IsValid())
		{
			AssetLoader.Unload(EnemyData->AnimationBlueprint.ToSoftObjectPath());
		}*/

		if (EnemyData->AttackMontage.IsValid())
		{
			AssetLoader.Unload(EnemyData->AttackMontage.ToSoftObjectPath());
		}
	}

	for (const auto& [EnemyName, EnemyArray] : ActiveObjects)
	{
		for (const auto& Enemy : EnemyArray)
		{
			auto Controller = Enemy->GetController();
			Controller->Destroy();
			Enemy->Destroy();
		}
	}

	for (const auto& [EnemyName, EnemyArray] : InactiveObjects)
	{
		for (const auto& Enemy : EnemyArray)
		{
			auto Controller = Enemy->GetController();
			Controller->Destroy();
			Enemy->Destroy();
		}
	}

	ActiveObjects.Empty();
	InactiveObjects.Empty();
}
