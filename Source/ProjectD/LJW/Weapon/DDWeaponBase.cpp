// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponBase.h"
#include "YSY/Game/DDGameSingleton.h"
#include "LJW/Animation/DDVisibleWeaponAnimNotify.h"
#include "LJW/Animation/DDHiddenWeaponAnimNotify.h"

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

	//Equip Montage
	if (WeaponData.EquipMontage)
	{
		EquipWeaponAnim = WeaponData.EquipMontage;
		UE_LOG(LogTemp, Warning, TEXT("Montage : %s"), *EquipWeaponAnim->GetFName().ToString());

		TArray<FAnimNotifyEvent> notifyEvents{ EquipWeaponAnim->Notifies };

		for (FAnimNotifyEvent eventNotify : notifyEvents)
		{
			UDDVisibleWeaponAnimNotify* startNotify = Cast<UDDVisibleWeaponAnimNotify>(eventNotify.Notify);

			if (startNotify)
			{
				startNotify->OnVisibleDelegatge.AddUObject(this, &ADDWeaponBase::EnableWeapon);
			}
		}
	}

	//Unequip Montage
	if (WeaponData.UnequipMontage)
	{
		UnequipWeaponAnim = WeaponData.UnequipMontage;
		
		UE_LOG(LogTemp, Warning, TEXT("Montage : %s"), *UnequipWeaponAnim->GetFName().ToString());
		TArray<FAnimNotifyEvent> notifyEvents{ UnequipWeaponAnim->Notifies };

		for (FAnimNotifyEvent eventNotify : notifyEvents)
		{
			if (UDDHiddenWeaponAnimNotify* startNotify = Cast<UDDHiddenWeaponAnimNotify>(eventNotify.Notify))
			{
				startNotify->OnHiddenDelegatge.AddUObject(this, &ADDWeaponBase::DisableWeapon);
			}
		}
	}
	
	//Mesh
	if (WeaponData.WeaponMesh) 
	{
		WeaponSkeletal->SetSkeletalMesh(WeaponData.WeaponMesh);
		WeaponSkeletal->SetCollisionProfileName(TEXT("NoCollision"));
	}
	//무기 비활성화
	DisableWeapon();
	
	//Skill Montage
	if (WeaponData.SkillMontage)
	{
		SkillWeaponAnim = WeaponData.SkillMontage;
	}
}

