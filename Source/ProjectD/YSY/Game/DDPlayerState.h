// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DDPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADDPlayerState();

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE bool CheckGold(const int32 InGold) const { return Gold>=InGold; }
	FORCEINLINE void AddGold(const int32 InGold)
	{ 
		Gold += InGold;
		
	}
	FORCEINLINE bool SubtractGold(const int32 InGold)
	{
		if (CheckGold(InGold)) {
			Gold -= InGold;
			return true;
		}
		else {
			return false;
		}
	}

	FORCEINLINE int32 GetLikePoint() const { return LikePoint; }
	FORCEINLINE bool CheckLikePoint(const int32 InLikePoint) const { return LikePoint >= InLikePoint; }
	FORCEINLINE void AddLikePoint(const int32 InLikePoint)
	{
		LikePoint += InLikePoint;

	}
	FORCEINLINE bool SubtractLikePoint(const int32 InLikePoint)
	{
		if (CheckLikePoint(InLikePoint)) {
			LikePoint -= InLikePoint;
			return true;
		}
		else {
			return false;
		}
	}

	

private:
	int32 Gold;
	int32 LikePoint;
};
