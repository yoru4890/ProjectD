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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE bool AddGold(const int32 InGold)
	{ 
		if (Gold + InGold >= 0) {
			Gold += InGold;
			return true;
		}
		else {
			return false;
		}
		
	}

	

private:
	UPROPERTY(EditAnywhere)
	int32 Gold;
};
