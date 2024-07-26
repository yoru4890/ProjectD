// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "DDWeaponSystemComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Cudgel,
	LightSaber,
	Sword,
	Rifle,
	Sniper,
	Bazooka,
	Machinegun,
	Unknown
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UDDWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDDWeaponSystemComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeWeapon();
		

private:
	
	class ADDWeaponBase* Weapon1; // ¸ùµÕÀÌ
	class ADDWeaponBase* Weapon2;
	ADDWeaponBase* Weapon3;
	ADDWeaponBase* Weapon4;
	ADDWeaponBase* Weapon5;
	ADDWeaponBase* Weapon6;
	ADDWeaponBase* Weapon7;
	TArray<ADDWeaponBase*> Weapons;
};
