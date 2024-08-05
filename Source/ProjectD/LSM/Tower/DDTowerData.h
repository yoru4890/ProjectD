// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/DDBuildingBaseData.h"
#include "DDTowerData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDDTowerData : public FDDBuildingBaseData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TowerBuildCost; // 타워의 설치비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TowerUpgradeCost; // 타워의 업그레이드 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TowerUnlockCost; // 타워의 언락 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TowerCoolTime; // 타워의 공격 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TowerDamage; // 타워의 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TowerLevel; // 타워의 레벨(테크)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TowerParentRowName; // 부모 타워의 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> TowerChildRowNames; // 자식 타워의 이름들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTowerUnlocked; // 타워가 언락되었는지 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDotTower; // 도트 공격 타워 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTower"))
	float DotDamage; // 도트 공격 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTower"))
	float DotDuration; // 도트 공격 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsDotTower"))
	float DotInterval; // 도트 공격 간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSlowTower; // 이동 속도 감소 트랩 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTower"))
	float SlowAmount; // 이동 속도 감소 비율 (0.0 ~ 1.0, 예: 0.5는 50% 감소)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsSlowTower"))
	float SlowDuration; // 이동 속도 감소 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADDTowerBase> TowerClass; // 타워 클래스
};
