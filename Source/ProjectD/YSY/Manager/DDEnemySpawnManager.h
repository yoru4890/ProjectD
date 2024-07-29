// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDEnemySpawnManager.generated.h"

class ADDEnemyBase;

UCLASS()
class PROJECTD_API UDDEnemySpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	UDDEnemySpawnManager();

	void Initialize(const TMap<FName, int32>& EnemyPoolSizes);
	AActor* Activate(const FName& EnemyName, const FVector& Position);
	void Deactivate(const FName& EnemyName, ADDEnemyBase* Enemy);

	UFUNCTION()
	void OnEnemyDestoryed(AActor* DestoryedActor);

	void SpawnEnemy(const FName& EnemyName);

private:
	TMap<FName, TArray<ADDEnemyBase*>> Pools;
	TMap<FName, TArray<ADDEnemyBase*>> ActiveObjects;
	TMap<FName, TArray<ADDEnemyBase*>> InactiveObjects;
};
