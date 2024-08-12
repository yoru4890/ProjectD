// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDBuildingBaseAttackStrategy.h"
#include "DDMachineGunTowerAttackStrategy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTD_API UDDMachineGunTowerAttackStrategy : public UDDBuildingBaseAttackStrategy
{
	GENERATED_BODY()

public:
	virtual void Attack(AActor* TargetEnemy) override;
	
private:


};
