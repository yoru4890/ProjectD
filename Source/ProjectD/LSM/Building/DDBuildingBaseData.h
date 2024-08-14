// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // Ensure this include is present for FTableRowBase
#include "DDBuildingBaseData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	Trap UMETA(DisplayName = "Trap"),
	Tower UMETA(DisplayName = "Tower"),
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
	EBuildingType BuildingType; // ������Ʈ�� ���� �ĺ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OccupiedCellWidth; // �׸��带 �����ϴ� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshZAxisModify; // z�� ���� ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAnimated; // �޽� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuildCost; //��ġ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeCost; //���׷��̵� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UnlockCost; //��� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolTime; //���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage; //������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ParentRowName; // �θ� ������ �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ChildRowNames; // �ڽ� ������ �̸���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUnlocked; // ����Ǿ����� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDot; // ��Ʈ ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDamage; // ��Ʈ ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDuration; // ��Ʈ ���� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotInterval; // ��Ʈ ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlow; // �̵� �ӵ� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlow"))
	float SlowAmount; // �̵� �ӵ� ���� ���� (0.0 ~ 1.0, ��: 0.5�� 50% ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlow"))
	float SlowDuration; // �̵� �ӵ� ���� ���� �ð�

	// Static mesh array, visible only if MeshType is StaticMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UStaticMesh>> StaticMeshs;

	// Skeletal mesh array, visible only if MeshType is SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<USkeletalMesh>> SkeletalMeshs;

	// Animation blueprint, visible only if MeshType is SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsAnimated == true"))
	TArray<TSoftObjectPtr<UAnimBlueprint>> AnimBlueprints;

	// Animation blueprint, visible only if MeshType is SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsAnimated == true"))
	TArray<TSoftObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UNiagaraSystem> AttackEffect; // ���� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UNiagaraSystem> HitEffect; // ���� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // ���丮 Ŭ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType; // ������ Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UDDBuildingBaseAttackStrategy> AttackStrategy; // ���� ����

	bool bIsLoading = false;
	bool bIsLoaded = false;
};
