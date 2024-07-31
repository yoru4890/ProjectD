// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponBase.h"
#include "YSY/Game/DDGameSingleton.h"

// Sets default values
ADDWeaponBase::ADDWeaponBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	WeaponSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponSkeletal->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADDWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADDWeaponBase::DisableWeapon()
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ADDWeaponBase::EnableWeapon()
{
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
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

	////Mesh
	if (WeaponData.WeaponMesh) 
	{
		WeaponSkeletal->SetSkeletalMesh(WeaponData.WeaponMesh);
		WeaponSkeletal->SetCollisionProfileName(TEXT("NoCollision"));
	}

	//무기 비활성화
	DisableWeapon();
	
}



