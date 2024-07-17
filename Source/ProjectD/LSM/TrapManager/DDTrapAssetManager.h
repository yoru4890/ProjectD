// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDTrapAssetManager.generated.h"

/**
 * TODO 트리를 초기화 하는 경우와 같이 로딩된 에셋 초기화 하기 (공간복잡도 향상) / 쓸것 같은 에셋들만 비동기 로딩하는 방법으로 구현하기!
 */

USTRUCT()
struct FLoadedTrapAsset
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UStaticMesh> StaticMesh;

    UPROPERTY()
    TObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY()
    TObjectPtr<UAnimBlueprint> AnimBlueprint;

    UPROPERTY()
    TObjectPtr<UParticleSystem> Effect;

    FLoadedTrapAsset() {}
};

USTRUCT(BlueprintType)
struct FTrapAssetsToLoad
{
    GENERATED_BODY()

    UPROPERTY()
    FSoftObjectPath StaticMesh;

    UPROPERTY()
    FSoftObjectPath SkeletalMesh;

    UPROPERTY()
    FSoftObjectPath Effect;

    UPROPERTY()
    FSoftObjectPath AnimBlueprint;

    FTrapAssetsToLoad() {}
};

UCLASS()
class PROJECTD_API UDDTrapAssetManager : public UObject
{
	GENERATED_BODY()

public:
    UDDTrapAssetManager();
    // TrapAsset을 비동기로 불러오는 함수
    void LoadTrapAssetsAsync();

    // Static Mesh를 불러오는 함수
    UStaticMesh* GetStaticMesh(const FName& TrapName);

    // Skeletal Mesh를 불러오는 함수
    USkeletalMesh* GetSkeletalMesh(const FName& TrapName);

    // Animation Blueprint를 불러오는 함수
    UAnimBlueprint* GetAnimBlueprint(const FName& TrapName);

    // Particle Effect를 불러오는 함수
    UParticleSystem* GetParticleEffect(const FName& TrapName);

private:

    // Asset이 로드되면 실행될 콜백함수
    void OnTrapAssetsLoaded();


    template<typename AssetType>
    void LoadAssetIfValid(UDDGameInstance* MyGameInstance, const FSoftObjectPath& AssetPath, TObjectPtr<AssetType>& OutLoadedAsset);

    // 로딩해야할 Asset을 담고 있는 자료구조
    UPROPERTY()
    TMap<FName, FTrapAssetsToLoad> TrapAssetsToLoads;

    // 로딩된 Asset을 담고 있는 자료구조 
    // TODO sds
    UPROPERTY()
    TMap<FName, FLoadedTrapAsset> LoadedTrapAssets;

};
