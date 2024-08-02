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

	UFUNCTION(BlueprintCallable)
	void SetSplines();

	UFUNCTION(BlueprintCallable)
	TArray<class AAISplineRoute*>& GetSplines() { return Splines; }

	UFUNCTION(BlueprintCallable)
	TArray<FDDWaveData>& GetStageWaveInfos() { return StageWaveInfo; }

private:

	TArray<FDDWaveData> StageWaveInfo;

	TArray<class AAISplineRoute*> Splines;
};
