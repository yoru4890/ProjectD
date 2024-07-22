// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/DDEnemyManager.h"
#include "YSY/Enemy/DDEnemyBase.h"
#include "YSY/Game/DDGameSingleton.h"

UDDEnemyManager::UDDEnemyManager()
{
}

void UDDEnemyManager::OnEnemyDestoryed(AActor* DestoryedActor)
{
	// TODO : Player should get Gold, Score
}

void UDDEnemyManager::SpawnEnemy(const FName& EnemyName)
{
	// TODO : Setting SpawnTransform

	ADDEnemyBase* Enemy = GetWorld()->SpawnActorDeferred<ADDEnemyBase>(ADDEnemyBase::StaticClass(), {});

	if (Enemy)
	{
		Enemy->OnDestroyed.AddDynamic(this, &UDDEnemyManager::OnEnemyDestoryed);
		Enemy->InitializeEnemy(*UDDGameSingleton::Get().GetEnemyDataTable().Find(EnemyName));
		Enemy->FinishSpawning({});
	}
}
