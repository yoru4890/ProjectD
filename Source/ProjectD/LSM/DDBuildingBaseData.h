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
	FString DisplayName; // ������Ʈ�� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID; // ������Ʈ�� ���� �ĺ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OccupiedCellWidth; // �׸��带 �����ϴ� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshZAxisModify; // z�� ���� ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeshType MeshType; // �޽� Ÿ��


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
	TArray<TSoftObjectPtr<UParticleSystem>> Effects; // ���� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // ���丮 Ŭ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType; // ������ Ÿ��

	bool bIsLoading = false;
	bool bIsLoaded = false;
};
