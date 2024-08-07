// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
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

	FORCEINLINE class UDDAssetManager* GetAssetManager() const { return AssetManager; }

	FORCEINLINE class UDDBuildingManager* GetBuildingManager() const { return BuildingManager; }

	FORCEINLINE class UDDFactoryManager* GetFactoryManager() const { return FactoryManager; }

	FORCEINLINE FStreamableManager& GetStreamableManager() { return StreamableManager; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDAssetManager> AssetManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDBuildingManager> BuildingManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDFactoryManager> FactoryManager;

	FStreamableManager StreamableManager;


};
