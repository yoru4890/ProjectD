// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/DDBuildingBase.h"
#include "LSM/Building/DDBuildingAttackInterface.h"
#include "DDTrap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDTrap : public ADDBuildingBase , public IDDBuildingAttackInterface
{
	GENERATED_BODY()
public:
	ADDTrap();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Attack() override;

protected:
	virtual void ModifyMeshAndAttackCollision() const override;

	virtual void ResetCanAttack() override;

	virtual void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
