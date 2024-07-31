// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "DDWeaponSystemComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Cudgel UMETA(DisplayName = "Cudgel"),
	LightSaber UMETA(DisplayName = "LightSaber"),
	Sword UMETA(DisplayName = "Sword"),

	Rifle UMETA(DisplayName = "Rifle"),
	Sniper UMETA(DisplayName = "Sniper"),
	Bazooka UMETA(DisplayName = "Bazooka"),
	Machinegun UMETA(DisplayName = "Machinegun"),
	
	Unknown UMETA(DisplayName = "Unknown")
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
	
	void EquipMeleeWeapon();
	void EquipRangeWeapon();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TArray<ADDWeaponBase*> Weapons;

	//들고있는 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADDWeaponBase> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	int32 CurrentMeleeWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	int32 CurrentRangeWeapon;


};
