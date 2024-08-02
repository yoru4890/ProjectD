// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponSystemComponent.h"
#include "YSY/Game/DDGameSingleton.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include <Kismet/KismetSystemLibrary.h>



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
	// �̱��濡�� data���� ����, ������ ���� �߿�
	auto WeaponDatas = UDDGameSingleton::Get().GetWeaponDataTable();
	
	for (auto& [WeaponDataName, WeaponDataValue] : WeaponDatas)
	{
		UClass* TempClass = WeaponDataValue.WeaponClass;
		check(TempClass);

		//���� Ŭ���� ����
		auto TempWeapon = GetWorld()->SpawnActor<ADDWeaponBase>(TempClass, FVector::ZeroVector, FRotator::ZeroRotator);
		check(TempWeapon);
		UE_LOG(LogTemp, Warning, TEXT("Spawn Weapon : %s"), *WeaponDataName.ToString());

		//Player�� ���� ����
		if (GetOwner())
		{
			//FindComponentByClass -> ���� ���� ��� ���� ù ��° �޽� ��ȯ
			ParentSkeletal = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
			
			TempWeapon->AttachToComponent(ParentSkeletal, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RifleSocket"));
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is null"));
		}
		
		//���� Data
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
		PlayUnequipMontage();

		CurrentWeapon = Weapons[CurrentMeleeWeapon];
		UE_LOG(LogTemp, Warning, TEXT("Current Weapon : %s"), *CurrentWeapon->GetFName().ToString());

	}
}

void UDDWeaponSystemComponent::EquipRangeWeapon()
{
	if (CurrentWeapon != Weapons[CurrentRangeWeapon])
	{
		PlayUnequipMontage();

		CurrentWeapon = Weapons[CurrentMeleeWeapon];
		UE_LOG(LogTemp, Warning, TEXT("Current Weapon : %s"), *CurrentWeapon->GetFName().ToString());
	}

}

void UDDWeaponSystemComponent::PlayEquipMontage()
{
	if (CurrentWeapon->GetEquipWeaponMontage())
	{
		ParentSkeletal->GetAnimInstance()->Montage_Play(CurrentWeapon->GetEquipWeaponMontage());
	}
}

void UDDWeaponSystemComponent::PlayUnequipMontage()
{
	if (CurrentWeapon->GetUnequipWeaponMontage())
	{
		ParentSkeletal->GetAnimInstance()->Montage_Play(CurrentWeapon->GetUnequipWeaponMontage());
	}
}



