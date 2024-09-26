// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DDSaveGame.generated.h"

UCLASS()
class PROJECTD_API UDDSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UDDSaveGame();

	FORCEINLINE const FString GetSaveSlotName() const noexcept { return SaveSlotName; }
	FORCEINLINE const int32 GetCurrentLikePoint() const noexcept { return CurrentLikePoint; }
	FORCEINLINE const TMap<FName, bool> GetBuildingUnlocked() const noexcept { return BuildingUnlocked; }
	FORCEINLINE const TArray<int32> GetStageLikePoints() const noexcept { return StageLikePoints; }
	FORCEINLINE const TArray<bool> GetStageCleared() const noexcept { return StageCleared; }


	void SetSaveSlotName(const FString& NewSaveSlotName) { SaveSlotName = NewSaveSlotName; }
	void SetCurrentLikePoint(const int32& NewCurrentLikePoint) { CurrentLikePoint = NewCurrentLikePoint; }
	void SetBuildingLockState(const FName& BuildingName, const bool& bIsUnlocked);
	void SetStageLikePoint(const int32& CurrentStage, const int32& LikePoint);
	void SetStageCleared(const int32& CurrentStage, const bool& bIsCleared);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	TArray<int32> StageLikePoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	TArray<bool> StageCleared;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	int32 TotalLikePoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	int32 CurrentLikePoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	TMap<FName, bool> BuildingUnlocked;
};
