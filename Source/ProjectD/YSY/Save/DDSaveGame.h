// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DDSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSaveGameData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int32> StageLikePoints;

	UPROPERTY()
	TArray<bool> StageCleared;

	UPROPERTY()
	int32 TotalLikePoint;

	UPROPERTY()
	TMap<FName, bool> TrapUnlocked;

	UPROPERTY()
	TMap<FName, bool> TowerUnlocked;

	FSaveGameData& operator=(const FSaveGameData& Other)
	{
		if (this != &Other)
		{
			StageLikePoints = Other.StageLikePoints;
			StageCleared = Other.StageCleared;
			TotalLikePoint = Other.TotalLikePoint;
			TrapUnlocked = Other.TrapUnlocked;
			TowerUnlocked = Other.TowerUnlocked;
		}

		return *this;
	}
};

UCLASS()
class PROJECTD_API UDDSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const FString GetSaveSlotName() const noexcept { return SaveSlotName; }
	
	FORCEINLINE const int32 GetSaveIndex() const noexcept { return SaveIndex; }

	FORCEINLINE const FSaveGameData GetSaveGameData() const noexcept { return SaveGameData; }

	void SetSaveSlotName(const FString& NewSaveSlotName) { SaveSlotName = NewSaveSlotName; }
	void SetSaveIndex(const int32& NewSaveIndex) { SaveIndex = NewSaveIndex; }
	void SetSaveGameData(const FSaveGameData& NewSaveGameData) { SaveGameData = NewSaveGameData; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	int32 SaveIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData", Meta = (AllowPrivateAccess = "true"))
	FSaveGameData SaveGameData;
};
