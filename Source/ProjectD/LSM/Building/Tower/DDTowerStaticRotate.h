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

	virtual void SetCanAttack(bool bIsCanAttack) override;

private:
	// Ÿ�̸ӷ� ȣ��� �Լ�
	void RotateTowardsEnemy();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDRotationComponent> DDRotationComponent;

	FTimerHandle RotationTimerHandle;
};
