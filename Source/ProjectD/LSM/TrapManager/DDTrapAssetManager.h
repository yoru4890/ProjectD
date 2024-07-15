// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
    // TrapAsset을 비동기로 불러오는 함수
    void LoadTrapAssetsAsync();

private:

    // Asset이 로드되면 실행될 콜백함수
    void OnTrapAssetsLoaded();

    // TrapAsset의 경로를 저장고있는 배열
    UPROPERTY()
    TMap<FName, FSoftObjectPath> TrapAssetsToLoad;

    UPROPERTY()
    TObjectPtr<UGameInstance> GameInstance;

    // TrapAsset이 담겨있는 Mesh
    UPROPERTY()
    TMap<FName, TObjectPtr<UStaticMesh>> LoadedTrapStaticMeshes;

    UPROPERTY()
    TMap<FName, TObjectPtr<USkeletalMesh>> LoadedTrapSkeletalMeshes;
	
};
