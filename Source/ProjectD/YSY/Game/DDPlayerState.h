// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "YSY/Save/DDSaveGame.h"
#include "DDPlayerState.generated.h"


// 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, int32, CurrentGold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLikePointChanged, int32, CurrentLikePoint);
/**
 * 
 */

UCLASS()
class PROJECTD_API ADDPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADDPlayerState();

	// 델리게이트 선언
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGoldChanged OnGoldChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLikePointChanged OnLikePointChanged;

	UFUNCTION(BlueprintCallable, Category = "Gold")
	FORCEINLINE int32 GetGold() const { return Gold; }

	UFUNCTION(BlueprintCallable, Category = "Gold")
	FORCEINLINE bool CheckGold(const int32 InGold) const { return Gold>=InGold; }

	void AddGold(const int32 InGold);

	bool SubtractGold(const int32 InGold);

	UFUNCTION(BlueprintCallable, Category = "LikePoint")
	FORCEINLINE int32 GetLikePoint() const { return LikePoint; }

	UFUNCTION(BlueprintCallable, Category = "LikePoint")
	FORCEINLINE bool CheckLikePoint(const int32 InLikePoint) const { return LikePoint >= InLikePoint; }

	UFUNCTION(BlueprintCallable, Category = "LikePoint")
	void AddLikePoint(const int32 InLikePoint);

	bool SubtractLikePoint(const int32 InLikePoint);

	const int32 GetScore() const noexcept { return Score; }

	void SetScore(const int32& NewScore) { Score = NewScore; }


	// Save
	UFUNCTION(BlueprintCallable, Category = "Save")
	void AutoSaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save")
	void CreateSaveFile(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save")
	UDDSaveGame* GetSaveGame() const { return DDSaveGame; }

	UFUNCTION(BlueprintCallable, Category = "Save")
	void UpdateBuildingLockState(const FName& BuildingName, const bool& bIsUnlocked);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void UpdateStageLikePoint(const int32& CurrentStage, const int32& LikePoint);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void UpdateStageCleared(const int32& CurrentStage, const bool& bIsCleared);

	void InitState();

private:
	int32 Gold;
	int32 LikePoint;
	int32 Score;

	int32 LoadedRifleAmmo;
	int32 UnLoadedRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSaveGame> DDSaveGame;
};
