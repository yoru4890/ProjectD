// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDProjectileData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDDProjectileData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	TSubclassOf<class ADDProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftObjectPtr<class UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSoftObjectPtr<class UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSoftObjectPtr<class UNiagaraSystem> TrailEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TSoftObjectPtr<class USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TSoftObjectPtr<class USoundBase> FlyingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // 팩토리 클래스

	bool bIsLoaded = false;

	int32 PoolSize;
	
};
