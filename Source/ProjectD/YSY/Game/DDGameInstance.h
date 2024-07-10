// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	FORCEINLINE TObjectPtr<class UDDTrapAssetManager> GetTrapAssetManager() const { return TrapAssetManager; }

	FORCEINLINE TObjectPtr<class UDDTrapManager>  GetTrapManager() const { return TrapManager; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDTrapAssetManager> TrapAssetManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDTrapManager> TrapManager;

	
};
