// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "DDTowerData.generated.h"


/**
 * 
 */

UENUM(BlueprintType)
enum class ETowerType : uint8
{
	MachineGunTower UMETA(DisplayName = "MachineGunTower"),
	TankTower UMETA(DisplayName = "TankTower"),
	MissileTower UMETA(DisplayName = "MissileTower"),
	Unknown
};

USTRUCT(BlueprintType)
struct FDDTowerData : public FDDBuildingBaseData
{
	GENERATED_BODY()
	
public:
	FDDTowerData() : TowerType(ETowerType::Unknown), BarrelMeshOffset(FVector::ZeroVector), WaistMeshOffset(FVector::ZeroVector), TowerAttackRange(0.0f), bCanRecoil(false), MaxRecoilTime(0.0f), RecoilDistance(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADDTowerBase> TowerClass; // Ÿ�� Ŭ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETowerType TowerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BarrelMeshOffset; // �跲�Ž� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WaistMeshOffset; // �㸮�޽� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TowerAttackRange; // Ÿ�� ���ݹ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRecoil; // �ݵ�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRecoil"))
	float MaxRecoilTime; // �ݵ� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCanRecoil"))
	float RecoilDistance; // �ݵ� �Ÿ�

};
