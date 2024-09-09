// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/AttackStrategies/DDBaseAttackStrategy.h"
#include "DDThornTrapAttackStrategy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDThornTrapAttackStrategy : public UDDBaseAttackStrategy
{
	GENERATED_BODY()

public:
	virtual void Attack(AActor* TargetEnemy, UStaticMeshComponent* FireStaticMesh) override;
	
};
