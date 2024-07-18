// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/BaseStruct.h"
#include "UObject/NoExportTypes.h"
#include "DDAssetManager.generated.h"

/**
 * 
 */
USTRUCT()
struct FLoadedAsset
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<TObjectPtr<UStaticMesh>> StaticMeshs;

    UPROPERTY()
    TArray<TObjectPtr<USkeletalMesh>> SkeletalMeshs;

    UPROPERTY()
    TObjectPtr<UAnimBlueprint> AnimBlueprint;

    UPROPERTY()
    TArray<TObjectPtr<UParticleSystem>> Effects;

    FLoadedAsset() {}
};

UCLASS()
class PROJECTD_API UDDAssetManager : public UObject
{
	GENERATED_BODY()
	
public:
    void Initialize();

    void LoadAssetsAsync(const FName& RowName, const FBaseStruct& ObjectStruct);

    FLoadedAsset& GetLoadedAssetByName(const FName& RowName);

    void RemoveLoadedAssetByName(const FName& RowName);

    void RevoeLoadedAssetAll();

private:
    void OnAssetsLoaded(const FName& RowName, FLoadedAsset& LoadedAsset);

    // 로딩된 Asset을 담고 있는 자료구조 
    UPROPERTY()
    TMap<FName, FLoadedAsset> LoadedAssets;
};
