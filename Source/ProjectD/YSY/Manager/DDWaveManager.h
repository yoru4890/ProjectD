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

	TArray<class AAISplineRoute*>& GetSplines() { return Splines; };

private:

	TArray<TArray<FDDWaveData*>> WaveInfo;

	TArray<class AAISplineRoute*> Splines;
};
