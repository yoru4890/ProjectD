// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDTrapStruct.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class ETrapMeshType : uint8
{
	StaticMesh,
	SkeletalMesh,
	Unknown
};

UENUM(BlueprintType)
enum class ETrapType : uint8
{
	ThornTrap,
	IceTrap,
	Unknown
};

USTRUCT(BlueprintType)
struct FDDTrapStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapName; // Ʈ���� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETrapType TrapType; // Ʈ���� �޽� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapBuildCost; // Ʈ���� ��ġ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapUpgradeCost; // Ʈ���� ���׷��̵� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapUnlockCost; // Ʈ���� ��� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapCoolTime; // Ʈ���� ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapDamage; // Ʈ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapLevel; // Ʈ���� ����(��ũ)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapParentName; // �θ� Ʈ���� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> TrapChildNames; // �ڽ� Ʈ���� �̸���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTrapUnlocked; // Ʈ���� ����Ǿ����� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETrapMeshType TrapMeshType; // Ʈ���� �޽� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "TrapMeshType == ETrapMeshType::StaticMesh"))
	TSoftObjectPtr<UStaticMesh> TrapStaticMesh; // Ʈ���� ����ƽ �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "TrapMeshType == ETrapMeshType::SkeletalMesh"))
	TSoftObjectPtr<USkeletalMesh> TrapSkeletalMesh; // Ʈ���� ���̷��� �޽�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "TrapMeshType == ETrapMeshType::SkeletalMesh"))
	TSoftObjectPtr<UAnimBlueprint> TrapAnimBlueprint; // Ʈ���� �ִϸ��̼� �������Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UParticleSystem> TrapEffect; // Ʈ���� ���� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDotTrap; // ��Ʈ ���� Ʈ�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotDamage; // ��Ʈ ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotDuration; // ��Ʈ ���� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotInterval; // ��Ʈ ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlowTrap; // �̵� �ӵ� ���� Ʈ�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTrap"))
	float SlowAmount; // �̵� �ӵ� ���� ���� (0.0 ~ 1.0, ��: 0.5�� 50% ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTrap"))
	float SlowDuration; // �̵� �ӵ� ���� ���� �ð�

};