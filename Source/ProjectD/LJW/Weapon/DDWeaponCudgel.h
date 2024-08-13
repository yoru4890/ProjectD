// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "Components/CapsuleComponent.h"
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
	virtual void Attack() override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UCapsuleComponent* CollisionCapsule;

};
