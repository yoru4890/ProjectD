// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDTrapManager.generated.h"

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
struct FTrapStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapName; // 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapBuildCost; // 트랩의 설치비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapUpgradeCost; // 트랩의 업그레이드 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapUnlockCost; // 트랩의 언락 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapCoolTime; // 트랩의 공격 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapDamage; // 트랩의 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TrapLevel; // 트랩의 레벨(테크)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TrapParentName; // 부모 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> TrapChildNames; // 자식 트랩의 이름들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTrapUnlocked; // 트랩이 언락되었는지 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETrapMeshType TrapMeshType; // 트랩의 메쉬 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> TrapStaticMesh; // 트랩의 스태틱 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> TrapSkeletalMesh; // 트랩의 스켈레톤 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UParticleSystem> TrapEffect; // 트랩의 공격 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDotTrap; // 도트 공격 트랩 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotDamage; // 도트 공격 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotDuration; // 도트 공격 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTrap"))
	float DotInterval; // 도트 공격 간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlowTrap; // 이동 속도 감소 트랩 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTrap"))
	float SlowAmount; // 이동 속도 감소 비율 (0.0 ~ 1.0, 예: 0.5는 50% 감소)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTrap"))
	float SlowDuration; // 이동 속도 감소 지속 시간

};

UCLASS()
class PROJECTD_API UDDTrapManager : public UObject
{
	GENERATED_BODY()

public:
	UDDTrapManager();

	// 함정이 해금이 되었는지 체크하는 메서드
	UFUNCTION(BlueprintCallable)
	bool IsTowerUnlocked(const FName& TowerName) const;

	// 함정을 해금하는 메서드
	UFUNCTION(BlueprintCallable)
	void UnlockTower(const FName& TowerName);

	FTrapStruct* GetTrapData(const FName& TrapName) const;

	FORCEINLINE TObjectPtr<UDataTable> GetTrapDataTable() const { return TrapDataTable; }

private:
	// 해금된 함정 집합
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FName> UnlockedTowers;
	
	// 함정 정보
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> TrapDataTable;
};
