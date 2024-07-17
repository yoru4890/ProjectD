// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDTrapAssetManager.generated.h"

/**
 * TODO Ʈ���� �ʱ�ȭ �ϴ� ���� ���� �ε��� ���� �ʱ�ȭ �ϱ� (�������⵵ ���) / ���� ���� ���µ鸸 �񵿱� �ε��ϴ� ������� �����ϱ�!
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
    // TrapAsset�� �񵿱�� �ҷ����� �Լ�
    void LoadTrapAssetsAsync();

    // Static Mesh�� �ҷ����� �Լ�
    UStaticMesh* GetStaticMesh(const FName& TrapName);

    // Skeletal Mesh�� �ҷ����� �Լ�
    USkeletalMesh* GetSkeletalMesh(const FName& TrapName);

    // Animation Blueprint�� �ҷ����� �Լ�
    UAnimBlueprint* GetAnimBlueprint(const FName& TrapName);

    // Particle Effect�� �ҷ����� �Լ�
    UParticleSystem* GetParticleEffect(const FName& TrapName);

private:

    // Asset�� �ε�Ǹ� ����� �ݹ��Լ�
    void OnTrapAssetsLoaded();


    template<typename AssetType>
    void LoadAssetIfValid(UDDGameInstance* MyGameInstance, const FSoftObjectPath& AssetPath, TObjectPtr<AssetType>& OutLoadedAsset);

    // �ε��ؾ��� Asset�� ��� �ִ� �ڷᱸ��
    UPROPERTY()
    TMap<FName, FTrapAssetsToLoad> TrapAssetsToLoads;

    // �ε��� Asset�� ��� �ִ� �ڷᱸ�� 
    // TODO sds
    UPROPERTY()
    TMap<FName, FLoadedTrapAsset> LoadedTrapAssets;

};
