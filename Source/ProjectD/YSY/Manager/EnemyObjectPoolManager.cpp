// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/EnemyObjectPoolManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UEnemyObjectPoolManager::UEnemyObjectPoolManager()
{
}

void UEnemyObjectPoolManager::Initialize(const TMap<FName, TSubclassOf<AActor>>& Prefabs, int32 PoolSize)
{
	for (auto& [EnemyName, Enemy] : Prefabs)
	{
		Pools.FindOrAdd(EnemyName, TArray<AActor*>());
		ActiveObjects.FindOrAdd(EnemyName, TArray<AActor*>());
		InactiveObjects.FindOrAdd(EnemyName, TArray<AActor*>());

		for (int32 i{}; i < PoolSize; i++)
		{
		}
	}
}

AActor* UEnemyObjectPoolManager::Activate(const FName& EnemyName, const FVector& Position)
{
	return nullptr;
}

void UEnemyObjectPoolManager::Deactivate(const FName& EnemyName, AActor* Actor)
{
}
