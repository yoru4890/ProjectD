// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponSystemComponent.h"
#include "YSY/Game/DDGameSingleton.h"
#include "LJW/Weapon/DDWeaponRifle.h"

// Sets default values for this component's properties
UDDWeaponSystemComponent::UDDWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDDWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UDDWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDDWeaponSystemComponent::InitializeWeapon()
{
	// ΩÃ±€≈Êø°º≠ dataµÈ¿ª æÚ¿Ω
	auto WeaponDatas = UDDGameSingleton::Get().GetWeaponDataTable();
	Weapons.SetNum(static_cast<int32>(EWeaponType::Unknown));
	
	for (auto& [WeaponDataName, WeaponDataValue] : WeaponDatas)
	{
		UClass* TempClass = WeaponDataValue.WeaponClass;
		auto TempWeapon = GetWorld()->SpawnActor<ADDWeaponBase>(TempClass);
		TempWeapon->InitData(WeaponDataName,WeaponDataValue);
		Weapons.Add(TempWeapon);
	}

	// TODO : LJW MyInit()

}

