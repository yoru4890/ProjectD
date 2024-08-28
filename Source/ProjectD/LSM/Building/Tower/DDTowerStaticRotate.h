// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/Tower/DDTowerBase.h"
#include "DDTowerStaticRotate.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDTowerStaticRotate : public ADDTowerBase
{
	GENERATED_BODY()

public:
	ADDTowerStaticRotate();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void RotateTowardsEnemy();

	const bool IsEnemyInSight(float CosTheta = 0.97f) const;

	void SetIsNowAttack(bool InIsNowAttack);

	virtual void SetMeshs(const FDDBuildingBaseData& LoadedAsset) override;

	void StartRecoil();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDRotationComponent> DDRotationComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> BarrelStaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> WaistMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> PlatformMeshComponent;

	float RotationSpeed;

	bool IsNowAttack = false;
};
