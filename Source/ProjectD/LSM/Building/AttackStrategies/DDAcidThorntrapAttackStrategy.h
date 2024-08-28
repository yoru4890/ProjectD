// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/AttackStrategies/DDBaseAttackStrategy.h"
#include "DDAcidThorntrapAttackStrategy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDAcidThorntrapAttackStrategy : public UDDBaseAttackStrategy
{
	GENERATED_BODY()

public:
	virtual void Attack(AActor* TargetEnemy, const FVector& FireLocation = FVector(0, 0, 0), const FRotator& FireRotation = FRotator(0, 0, 0)) override;
};
