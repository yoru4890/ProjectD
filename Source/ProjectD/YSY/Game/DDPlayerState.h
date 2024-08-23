// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DDPlayerState.generated.h"


// 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoldChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLikePointChanged);

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

	void AddLikePoint(const int32 InLikePoint);
	bool SubtractLikePoint(const int32 InLikePoint);

	

private:
	int32 Gold;
	int32 LikePoint;
};
