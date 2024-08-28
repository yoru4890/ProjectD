// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Building/DDBuildingBaseData.h"
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
	TSubclassOf<class ADDTowerBase> TowerClass; // 타워 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TowerAttackRange; // 타워 공격범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRecoil; // 반동여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRecoil"))
	float MaxRecoilTime; // 반동 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRecoil"))
	float RecoilDistance; // 반동 거리

};
