// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/BaseStruct.h"
#include "DDTrapStruct.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class ETrapType : uint8
{
	ThornTrap,
	IceTrap,
	Unknown
};

USTRUCT(BlueprintType)
struct FDDTrapStruct : public FBaseStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETrapType TrapType; // Ʈ���� Ÿ��

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADDTrapBase> TrapClass; // Ʈ�� Ŭ����
};