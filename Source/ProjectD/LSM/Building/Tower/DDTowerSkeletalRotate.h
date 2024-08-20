// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/Tower/DDTowerBase.h"
#include "DDTowerSkeletalRotate.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDTowerSkeletalRotate : public ADDTowerBase
{
	GENERATED_BODY()

public:
	ADDTowerSkeletalRotate();

	virtual void Tick(float DeltaTime) override;

private:
	virtual void SetTargetEnemy(AActor* NewTargetEnemy) override;

	const bool IsEnemyInSight(float CosTheta = 0.97f) const;

	void SetIsNowAttack(bool InIsNowAttack);
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDRotationComponent> DDRotationComponent;

	bool IsNowAttack = false;
	
};
