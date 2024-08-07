// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/DDBuildingBase.h"
#include "DDTrap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDTrap : public ADDBuildingBase
{
	GENERATED_BODY()
public:
	ADDTrap();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Attack() override;
	virtual void ModifyMeshAndAttackCollision() const override;
};
