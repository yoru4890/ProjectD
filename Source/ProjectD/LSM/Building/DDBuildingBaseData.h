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
	FString DisplayName; // 오브젝트의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID; // 오브젝트의 고유 식별자

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBuildingType BuildingType; // 오브젝트의 고유 식별자

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OccupiedCellWidth; // 그리드를 차지하는 크기

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshZAxisModify; // z축 조정 값

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAnimated; // 메쉬 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuildCost; //설치비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeCost; //업그레이드 비용

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
	float DotDamage; // 도트 공격 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotDuration; // 도트 공격 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDot"))
	float DotInterval; // 도트 공격 간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlow; // 이동 속도 감소 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlow"))
	float SlowAmount; // 이동 속도 감소 비율 (0.0 ~ 1.0, 예: 0.5는 50% 감소)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlow"))
	float SlowDuration; // 이동 속도 감소 지속 시간

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
	TSoftObjectPtr<class UNiagaraSystem> AttackEffect; // 공격 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UNiagaraSystem> HitEffect; // 공격 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> FactoryClass; // 팩토리 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType; // 데미지 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UDDBuildingBaseAttackStrategy> AttackStrategy; // 공격 전략

	bool bIsLoading = false;
	bool bIsLoaded = false;
};
