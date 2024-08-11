// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "Components/BoxComponent.h"
#include "DDWeaponCudgel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDWeaponCudgel : public ADDWeaponBase
{
	GENERATED_BODY()

	ADDWeaponCudgel();
	
public:
	virtual void SubSkill() override;


public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent* CollisionBox;

};
