// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "DDWeaponRifle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDWeaponRifle : public ADDWeaponBase
{
	GENERATED_BODY()
	
	ADDWeaponRifle();
	virtual void BeginPlay() override;

public:
	virtual void SubSkill() override;
	virtual void Attack() override;

private:
	UPROPERTY()
	TObjectPtr<class APlayerCameraManager> CameraManager;
	
	const float AttackRange = 2500.0f;

};
