// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/DDBuildingBaseStruct.h"
#include "DDTrapStruct.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FDDTrapStruct : public FDDBuildingBaseStruct
{
	GENERATED_BODY()

public:
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
	FName TrapParentRowName; // 부모 트랩의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> TrapChildRowNames; // 자식 트랩의 이름들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTrapUnlocked; // 트랩이 언락되었는지 여부

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADDTrapBase> TrapClass; // 트랩 클래스
};