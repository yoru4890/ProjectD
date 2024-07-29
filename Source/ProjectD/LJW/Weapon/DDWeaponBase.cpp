// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponBase.h"
#include "YSY/Game/DDGameSingleton.h"

// Sets default values
ADDWeaponBase::ADDWeaponBase()
{
	
}

// Called when the game starts or when spawned
void ADDWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADDWeaponBase::InitData(const FName& RowName, const FDDWeaponData& WeaponData)
{
	//TODO : Init Data
	WeaponRowName = RowName;
	WeaponDisplayName = WeaponData.WeaponName;
	WeaponInformation = WeaponData.WeaponInformation;
	Price = WeaponData.Price;
	UpgradeCost = WeaponData.UpgradeCost;
	UnlockLP = WeaponData.UnlockLP;
	AttackCooltime = WeaponData.AttackCooltime;
	AttackDamage = WeaponData.AttackDamage;
	AttackRange = WeaponData.AttackRange;

	//Mesh
	WeaponMesh = WeaponData.WeaponMesh;
}



