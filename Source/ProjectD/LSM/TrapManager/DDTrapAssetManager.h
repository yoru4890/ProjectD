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

    UStaticMesh* GetStaticMesh(const FName& TrapName);
    USkeletalMesh* GetSkeletalMesh(const FName& TrapName);
    UAnimBlueprint* GetAnimBlueprint(const FName& TrapName);
    UParticleSystem* GetParticleEffect(const FName& TrapName);

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

    UPROPERTY()
    TMap<FName, TObjectPtr<UAnimBlueprint>> LoadedTrapAnimBlueprints;

    UPROPERTY()
    TMap<FName, TObjectPtr<UParticleSystem>> LoadedTrapEffects;
	
};
