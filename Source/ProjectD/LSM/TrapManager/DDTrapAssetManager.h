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
    // TrapAsset�� �񵿱�� �ҷ����� �Լ�
    void LoadTrapAssetsAsync();

private:

    // Asset�� �ε�Ǹ� ����� �ݹ��Լ�
    void OnTrapAssetsLoaded();

    // TrapAsset�� ��θ� ������ִ� �迭
    UPROPERTY()
    TMap<FName, FSoftObjectPath> TrapAssetsToLoad;

    UPROPERTY()
    TObjectPtr<UGameInstance> GameInstance;

    // TrapAsset�� ����ִ� Mesh
    UPROPERTY()
    TMap<FName, TObjectPtr<UStaticMesh>> LoadedTrapStaticMeshes;

    UPROPERTY()
    TMap<FName, TObjectPtr<USkeletalMesh>> LoadedTrapSkeletalMeshes;
	
};
