// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YSY/GameData/DDWaveData.h"
#include "DDWaveManager.generated.h"

DECLARE_DELEGATE_RetVal_TwoParams(AActor*, FOnActivateEnemySignature, const FName&, int32);
DECLARE_DELEGATE_OneParam(FOnSetupEnemyPoolSignature, int32);

UCLASS()
class PROJECTD_API UDDWaveManager : public UObject
{
	GENERATED_BODY()

public:
	UDDWaveManager();

	void Initialize();

	UFUNCTION(BlueprintCallable)
	void InitStage(int32 StageNum);

	UFUNCTION(BlueprintCallable)
	void SetSplines();

	UFUNCTION(BlueprintCallable)
	TArray<class AAISplineRoute*>& GetSplines() { return Splines; }

	UFUNCTION(BlueprintCallable)
	TArray<FDDWaveData>& GetStageWaveInfos() { return StageWaveInfo; }

	UFUNCTION(BlueprintCallable)
	void WaveStart();
	void SpawnEnd();
	void WaveEnd();

	void StageEnd();

	void AddEnemyNumber();
	void SubEnemyNumber();

	void InitEnemyNames();

public:
	FOnActivateEnemySignature OnActivateEnemySignature;
	FOnSetupEnemyPoolSignature OnSetupEnemyPoolSignature;

private:
	TArray<FDDWaveData> StageWaveInfo;

	TArray<class AAISplineRoute*> Splines;

	int32 CurrentStage{};
	int32 CurrentWave{};
	int32 EnemyIndex{};
	int32 TotalSpawnEnemyCount{};
	int32 TotalEnemyCount{};
	FTimerHandle WaveTimerHandle;

	bool bIsWaveInProgress{};

	TArray<FName> EnemyNames;
};
