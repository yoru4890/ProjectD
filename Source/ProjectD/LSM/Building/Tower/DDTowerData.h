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
	TSubclassOf<class ADDTowerBase> TowerClass; // Ÿ�� Ŭ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TowerAttackRange; // Ÿ�� ���ݹ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRecoil; // �ݵ�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRecoil"))
	float MaxRecoilTime; // �ݵ� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRecoil"))
	float RecoilDistance; // �ݵ� �Ÿ�

};
