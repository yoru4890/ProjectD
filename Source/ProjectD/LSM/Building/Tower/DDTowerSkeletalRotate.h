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

private:
	virtual void SetTargetEnemy(AActor* NewTargetEnemy) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDDRotationComponent> DDRotationComponent;
	
};
