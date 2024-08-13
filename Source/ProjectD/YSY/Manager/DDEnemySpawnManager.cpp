// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/DDEnemySpawnManager.h"
#include "YSY/Enemy/DDEnemyBase.h"
#include "YSY/Game/DDGameSingleton.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Manager/DDWaveManager.h"
#include "Kismet/GameplayStatics.h"

UDDEnemySpawnManager::UDDEnemySpawnManager()
{
}

void UDDEnemySpawnManager::SetupEnemyPools(int32 StageNum)
{
	FString StringNum = FString::Printf(TEXT("%d"), StageNum);
	FName StageName(*StringNum);
	const TMap<FName, int32>& EnemyPoolSizes = UDDGameSingleton::Get().GetWaveDataTable().Find(StageName)->EnemyPoolSizes;

	Pools.Empty();
	ActiveObjects.Empty();
	InactiveObjects.Empty();

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

AActor* UDDEnemySpawnManager::Activate(const FName& EnemyName, int32 SplineIndex)
{
	if (InactiveObjects.Contains(EnemyName) && InactiveObjects[EnemyName].Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("YES"));
		UE_LOG(LogTemp, Warning, TEXT("%d"), InactiveObjects[EnemyName].Num());
		UE_LOG(LogTemp, Warning, TEXT("%d"), ActiveObjects[EnemyName].Num());
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
		// TODO : YSY Player should get Gold, Score

		UE_LOG(LogTemp, Warning, TEXT("%d"), InactiveObjects[EnemyName].Num());
		UE_LOG(LogTemp, Warning, TEXT("%d"), ActiveObjects[EnemyName].Num());

	}
}

void UDDEnemySpawnManager::SpawnEnemy(const FName& EnemyName)
{
	// TODO : YSY Setting SpawnTransform

	ADDEnemyBase* Enemy = GetWorld()->SpawnActorDeferred<ADDEnemyBase>(ADDEnemyBase::StaticClass(), {});

	if (Enemy)
	{

		Enemy->OnDie.AddUObject(this, &UDDEnemySpawnManager::Deactivate);
		Enemy->InitializeEnemy(*UDDGameSingleton::Get().GetEnemyDataTable().Find(EnemyName));
		Enemy->FinishSpawning({});
		Enemy->Deactivate();
		Pools[EnemyName].Add(Enemy);
		InactiveObjects[EnemyName].Add(Enemy);
	}
}
