// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // Ensure this include is present for FTableRowBase
#include "DDBuildingBaseData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMeshType : uint8
{
	StaticMesh,
	SkeletalMesh,
	Unknown
};

USTRUCT(BlueprintType)
struct FDDBuildingBaseData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName; // 오브젝트의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID; // 오브젝트의 고유 식별자

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OccupiedCellWidth; // 그리드를 차지하는 크기

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshZAxisModify; // z축 조정 값

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeshType MeshType; // 메쉬 타입


	// Static mesh array, visible only if MeshType is StaticMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::StaticMesh"))
	TArray<TSoftObjectPtr<UStaticMesh>> StaticMeshs;

	// Skeletal mesh array, visible only if MeshType is SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::SkeletalMesh"))
	TArray<TSoftObjectPtr<USkeletalMesh>> SkeletalMeshs;

	// Animation blueprint, visible only if MeshType is SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::SkeletalMesh"))
	TSoftObjectPtr<UAnimBlueprint> MyAnimBlueprint;

	// Animation blueprint, visible only if MeshType is SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "MeshType == EMeshType::SkeletalMesh"))
	TArray<TSoftObjectPtr<UAnimMontage>> AnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UParticleSystem>> Effects; // 공격 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // 팩토리 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType; // 데미지 타입

	bool bIsLoading = false;
	bool bIsLoaded = false;
};
