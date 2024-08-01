// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponSystemComponent.h"
#include "YSY/Game/DDGameSingleton.h"
#include "LJW/Weapon/DDWeaponBase.h"



// Sets default values for this component's properties
UDDWeaponSystemComponent::UDDWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDDWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeWeapon();
}

void UDDWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDDWeaponSystemComponent::InitializeWeapon()
{
	// 싱글톤에서 data들을 얻음, 데이터 순서 중요
	auto WeaponDatas = UDDGameSingleton::Get().GetWeaponDataTable();
	
	for (auto& [WeaponDataName, WeaponDataValue] : WeaponDatas)
	{
		UClass* TempClass = WeaponDataValue.WeaponClass;
		check(TempClass);

		//무기 클래스 스폰
		auto TempWeapon = GetWorld()->SpawnActor<ADDWeaponBase>(TempClass, FVector::ZeroVector, FRotator::ZeroRotator);
		check(TempWeapon);
		UE_LOG(LogTemp, Warning, TEXT("Spawn Weapon : %s"), *WeaponDataName.ToString());

		//Player에 무기 장착
		if (GetOwner())
		{
			//FindComponentByClass -> 여러 개일 경우 가장 첫 번째 메쉬 반환
			USkeletalMeshComponent* GetParentMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
			TempWeapon->AttachToComponent(GetParentMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RifleSocket"));
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is null"));
		}
		
		//무기 Data
		TempWeapon->InitData(WeaponDataName,WeaponDataValue);
		Weapons.Emplace(TempWeapon);

	}

	//Initializing current weapon
	CurrentMeleeWeapon = static_cast<int32>(EWeaponType::Cudgel);
	CurrentRangeWeapon = static_cast<int32>(EWeaponType::Rifle);
	CurrentWeapon = Weapons[CurrentMeleeWeapon];
}

void UDDWeaponSystemComponent::EquipMeleeWeapon()
{
	if (CurrentWeapon != Weapons[CurrentMeleeWeapon])
	{
		CurrentWeapon = Weapons[CurrentMeleeWeapon];
		//TODO : LJW Anim Notify
		Weapons[CurrentRangeWeapon]->DisableWeapon();
		CurrentWeapon->EnableWeapon();
	}
	
	switch (CurrentWeaponEnum)
	{
	case EWeaponType::Cudgel:
		GetOwner()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->Montage_Play(ChangeMontage);
		break;
	case EWeaponType::LightSaber:
		break;
	case EWeaponType::Sword:
		break;
	case EWeaponType::Rifle:
		break;
	case EWeaponType::Sniper:
		break;
	case EWeaponType::Bazooka:
		break;
	case EWeaponType::Machinegun:
		break;
	case EWeaponType::Unknown:
		break;
	default:
		break;
	}
}

void UDDWeaponSystemComponent::EquipRangeWeapon()
{
	if (CurrentWeapon != Weapons[CurrentRangeWeapon])
	{
		CurrentWeapon = Weapons[CurrentRangeWeapon];
		//TODO : LJW Anim Notify
		Weapons[CurrentMeleeWeapon]->DisableWeapon();
		CurrentWeapon->EnableWeapon();
		
	}
}



