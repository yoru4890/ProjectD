// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YSY/GameData/DDWaveData.h"
#include "DDWaveManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDWaveManager : public UObject
{
	GENERATED_BODY()

public:
	UDDWaveManager();

	void Initialize(class UDDGameInstance* GameInstance);

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
	void WaveEnd();

	void StageEnd();

private:
	TArray<FDDWaveData> StageWaveInfo;

	TArray<class AAISplineRoute*> Splines;

	int32 CurrentStage{};
	int32 CurrentWave{};
	int32 EnemyIndex{};
	int32 TotalEnemyCount{};
	FTimerHandle WaveTimerHandle;

	UPROPERTY()
	TObjectPtr<class UDDEnemySpawnManager> EnemySpawnManager{};
};
