// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StreamableManager.h"
#include "DDAssetManager.generated.h"

UCLASS()
class PROJECTD_API UDDAssetManager  : public UObject
{
	GENERATED_BODY()
	
public:
    void Initialize();

    void LoadAssetsAsync(const TArray<TSoftObjectPtr<UObject>>& AssetsToLoad, FStreamableDelegate OnAssetsLoadedCallback);

    void UnloadAsset(TArray<TSoftObjectPtr<UObject>>& AssetsToUnload);


private:

    bool ShouldForceGarbageCollection() const;

    int32 GetCurrentMemoryUsageInMB() const;
};
