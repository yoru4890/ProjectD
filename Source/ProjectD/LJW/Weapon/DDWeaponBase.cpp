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

void ADDWeaponBase::InitData(const FDDWeaponData& WeaponData)
{
	BuildCost = WeaponData.BuildCost;
}



