// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyObjectPoolManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UEnemyObjectPoolManager : public UObject
{
	GENERATED_BODY()
	
public:
	UEnemyObjectPoolManager();

	void Initialize(const TMap<FName, TSubclassOf<AActor>>& Prefabs, int32 PoolSize);
	AActor* Activate(const FName& EnemyName, const FVector& Position);
	void Deactivate(const FName& EnemyName, AActor* Actor);

private:
	TMap<FName, TArray<AActor*>> Pools;
	TMap<FName, TArray<AActor*>> ActiveObjects;
	TMap<FName, TArray<AActor*>> InactiveObjects;
};
