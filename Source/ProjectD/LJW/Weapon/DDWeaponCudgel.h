// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "DDWeaponCudgel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDWeaponCudgel : public ADDWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual void SubSkill() override;
};
