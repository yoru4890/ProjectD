// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDLoadedAsset.generated.h"

/**
 * 
 */
USTRUCT()
struct FDDLoadedAsset
{
    GENERATED_BODY()
public:
    UPROPERTY()
    TArray<TSoftObjectPtr<UStaticMesh>> StaticMeshs;

    UPROPERTY()
    TArray<TSoftObjectPtr<USkeletalMesh>> SkeletalMeshs;

    UPROPERTY()
    TSoftObjectPtr<UAnimBlueprint> AnimBlueprint;

    UPROPERTY()
    TArray<TSoftObjectPtr<UAnimMontage>> AnimMontages;

    UPROPERTY()
    TArray<TSoftObjectPtr<UParticleSystem>> Effects;

    bool IsLoading = false;  // 로딩 상태 관리 변수 추가

    FDDLoadedAsset() {}
};