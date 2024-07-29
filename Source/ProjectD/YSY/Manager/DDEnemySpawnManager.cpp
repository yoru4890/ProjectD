// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/DDEnemySpawnManager.h"
#include "YSY/Enemy/DDEnemyBase.h"
#include "YSY/Game/DDGameSingleton.h"

UDDEnemySpawnManager::UDDEnemySpawnManager()
{
}

void UDDEnemySpawnManager::Initialize(const TMap<FName, int32>& EnemyPoolSizes)
{
	for (auto& [EnemyName, PoolSize] : EnemyPoolSizes)
	{
		Pools.FindOrAdd(EnemyName, TArray<ADDEnemyBase*>());
		ActiveObjects.FindOrAdd(EnemyName, TArray<ADDEnemyBase*>());
		InactiveObjects.FindOrAdd(EnemyName, TArray<ADDEnemyBase*>());

		for (int32 i{}; i < PoolSize; i++)
		{
			SpawnEnemy(EnemyName);
		}
	}

}

AActor* UDDEnemySpawnManager::Activate(const FName& EnemyName, const FVector& Position)
{
	if (InactiveObjects.Contains(EnemyName) && InactiveObjects[EnemyName].Num() > 0)
	{
		ADDEnemyBase* Enemy = InactiveObjects[EnemyName].Last();
		InactiveObjects[EnemyName].Pop();
		ActiveObjects[EnemyName].Add(Enemy);
		Enemy->Activate();
		Enemy->SetActorLocation(Position);
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
	}
}

void UDDEnemySpawnManager::OnEnemyDestoryed(AActor* DestoryedActor)
{
	// TODO : YSY Player should get Gold, Score
}

void UDDEnemySpawnManager::SpawnEnemy(const FName& EnemyName)
{
	// TODO : YSY Setting SpawnTransform

	ADDEnemyBase* Enemy = GetWorld()->SpawnActorDeferred<ADDEnemyBase>(ADDEnemyBase::StaticClass(), {});

	if (Enemy)
	{
		Enemy->OnDestroyed.AddDynamic(this, &UDDEnemySpawnManager::OnEnemyDestoryed);
		Enemy->InitializeEnemy(*UDDGameSingleton::Get().GetEnemyDataTable().Find(EnemyName));
		Enemy->FinishSpawning({});
		Enemy->Deactivate();
		Pools[EnemyName].Add(Enemy);
		InactiveObjects[EnemyName].Add(Enemy);
	}
}
