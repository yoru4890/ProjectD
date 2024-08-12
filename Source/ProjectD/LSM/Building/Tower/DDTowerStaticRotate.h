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
	// 타이머로 호출될 함수
	void RotateTowardsEnemy();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDRotationComponent> DDRotationComponent;
};
