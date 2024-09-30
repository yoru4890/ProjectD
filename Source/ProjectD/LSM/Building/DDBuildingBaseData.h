// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "YSY/Interface/DamageInterface.h"
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
	FDDBuildingBaseData() : BuildingType(EBuildingType::Unknown), OccupiedCellWidth(0), MeshZAxisModify(0.0f), bIsAnimated(false), BuildCost(0), SellCost(0), UnlockCost(0), AttackCoolTime(0.0f), Damage(0.0f), bIsUnlocked(false), bIsDot(false), DotDamageType(EDotDamageType::Unknown), DotDamage(0.0f), DotDuration(0.0f), DotInterval(0.0f), bIsDebuff(false), DebuffType(EDebuffType::Unknown), DebuffRate(0.0f), DebuffDuration(0.0f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName; // ������Ʈ�� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectInfo; // ������Ʈ�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBuildingType BuildingType; // ������ Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> BuildingRealImage; // ������ ���� �̹���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> BuildingIconImage; // ������ ������ �̹���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OccupiedCellWidth; // �׸��带 �����ϴ� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshZAxisModify; // �׸��带 �����ϴ� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAnimated; // �޽� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuildCost; //��ġ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SellCost; //��ġ���

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
	EDotDamageType DotDamageType; // ��Ʈ ������ Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDamage; // ��Ʈ ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDuration; // ��Ʈ ���� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotInterval; // ��Ʈ ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDebuff; // ����� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDebuff"))
	EDebuffType DebuffType; // ����� Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDebuff"))
	float DebuffRate; // ����� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDebuff"))
	float DebuffDuration; // ����� ���ӽð�

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
	TSoftObjectPtr<UNiagaraSystem> AttackEffect; // ���� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> AttackSound; // ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> HitEffect; // ���� ����Ʈ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // ���丮 Ŭ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType; // ������ Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UDDBaseAttackStrategy> AttackStrategy; // ���� ����

	bool bIsLoading = false;
	bool bIsLoaded = false;
};
