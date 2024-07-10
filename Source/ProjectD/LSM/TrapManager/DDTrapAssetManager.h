// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StreamableManager.h"
#include "DDTrapAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDTrapAssetManager : public UObject
{
	GENERATED_BODY()

public:
    UDDTrapAssetManager();

    // 저장된 TrapAsset을 불러오는 함수
    TObjectPtr<UStaticMesh> GetLoadedTrapMesh(const FName& TowerName) const;
    // TrapAsset을 비동기로 불러오는 함수
    void LoadTrapAssetsAsync();

private:

    // Asset이 로드되면 실행될 콜백함수
    void OnTrapAssetsLoaded();

    // TrapAsset의 경로를 저장고있는 배열
    UPROPERTY()
    TMap<FName, FSoftObjectPath> TrapAssetsToLoad;

    // TrapAsset이 담겨있는 Mesh
    UPROPERTY()
    TMap<FName,UStaticMesh*> LoadedTrapMeshes;

    UPROPERTY()
    TObjectPtr<UGameInstance> GameInstance;

    FStreamableManager StreamableManager;
	
};
