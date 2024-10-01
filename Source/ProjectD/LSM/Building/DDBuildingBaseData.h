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
	FText DisplayName; // 오브젝트의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectInfo; // 오브젝트의 정보

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBuildingType BuildingType; // 빌딩의 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> BuildingRealImage; // 빌딩의 실제 이미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> BuildingIconImage; // 빌딩의 아이콘 이미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OccupiedCellWidth; // 그리드를 차지하는 크기

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshZAxisModify; // 그리드를 차지하는 크기

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAnimated; // 메쉬 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuildCost; //설치비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SellCost; //설치비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UnlockCost; //언락 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolTime; //공격 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage; //데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ParentRowName; // 부모 빌딩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ChildRowNames; // 자식 빌딩의 이름들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUnlocked; // 언락되었는지 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDot; // 도트 공격 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	EDotDamageType DotDamageType; // 도트 데미지 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDamage; // 도트 공격 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDuration; // 도트 공격 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotInterval; // 도트 공격 간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDebuff; // 디버프 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDebuff"))
	EDebuffType DebuffType; // 디버프 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDebuff"))
	float DebuffRate; // 디버프 비율

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDebuff"))
	float DebuffDuration; // 디버프 지속시간

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
	TSoftObjectPtr<UNiagaraSystem> AttackEffect; // 공격 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> AttackSound; // 공격 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> HitEffect; // 공격 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // 팩토리 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType; // 데미지 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UDDBaseAttackStrategy> AttackStrategy; // 공격 전략

	bool bIsLoading = false;
	bool bIsLoaded = false;
};
