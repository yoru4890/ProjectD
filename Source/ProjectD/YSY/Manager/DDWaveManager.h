// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YSY/GameData/DDWaveData.h"
#include "DDWaveManager.generated.h"

DECLARE_DELEGATE_RetVal_TwoParams(AActor*, FOnActivateEnemySignature, const FName&, int32);
DECLARE_DELEGATE_OneParam(FOnSetupEnemyPoolSignature, int32);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainingLivesChangedSignature, int32, CurrentRemainingLives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveChangedSignature, int32, CurrentWave, int32, MaxWave);

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

	const int32& GetRemainingLives() const noexcept { return RemainingLives; }
	const int32& GetCurrentWave() const noexcept { return CurrentWave; }
	const int32& GetMaxWave() const noexcept { return MaxWave; }
	UFUNCTION(BlueprintCallable)
	void WaveStart();

	UFUNCTION(BlueprintCallable)
	void WaveForceEnd();

	void SpawnEnd();
	void WaveEnd();

	void StageEnd();

	void AddEnemyNumber();
	void SubEnemyNumber();

	void InitEnemyNames();

	UFUNCTION(BlueprintCallable)
	void SubtractRemainingLives(int32 Amount);

public:
	FOnActivateEnemySignature OnActivateEnemySignature;
	FOnSetupEnemyPoolSignature OnSetupEnemyPoolSignature;
	FOnRemainingLivesChangedSignature OnRemainingLivesChanged;
	FOnWaveChangedSignature OnWaveChangedSignature;

private:
	TArray<FDDWaveData> StageWaveInfo;

	TArray<class AAISplineRoute*> Splines;

	int32 CurrentStage{};
	int32 CurrentWave{};
	int32 MaxWave{};
	int32 EnemyIndex{};
	int32 TotalSpawnEnemyCount{};
	int32 TotalEnemyCount{};
	int32 RemainingLives{};
	FTimerHandle WaveTimerHandle;

	bool bIsWaveInProgress{};

	TArray<FName> EnemyNames;

	TArray<int32> EnemyOrders;
	TArray<int32> GateOrders;

	TArray<TArray<int32>> PathGateMappings;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> VictoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	UUserWidget* VictoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> FailedWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	UUserWidget* FailedWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> WaveStartWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	UUserWidget* WaveStartWidget;
};
