// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponSystemComponent.h"
#include "YSY/Game/DDGameSingleton.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
			PlayerCharacter = Cast<ACharacter>(GetOwner());
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
	CurrentWeapon->EnableWeapon();

}

void UDDWeaponSystemComponent::EquipMeleeWeapon()
{
	if (CurrentWeapon != Weapons[CurrentMeleeWeapon])
	{
		OnSetWeaponIndexDelegate.Execute(CurrentMeleeWeapon);
		PlayUnequipMontage();

		UE_LOG(LogTemp, Warning, TEXT("Current Weapon : %s"), *CurrentWeapon->GetFName().ToString());

	}
}

void UDDWeaponSystemComponent::EquipRangeWeapon()
{
	if (CurrentWeapon != Weapons[CurrentRangeWeapon])
	{
		OnSetWeaponIndexDelegate.Execute(CurrentRangeWeapon);
		PlayUnequipMontage();

		UE_LOG(LogTemp, Warning, TEXT("Current Weapon : %s"), *CurrentWeapon->GetFName().ToString());
	}

}

void UDDWeaponSystemComponent::PlayEquipMontage()
{
	if (CurrentWeapon->GetEquipWeaponMontage())
	{
		CurrentWeapon = (CurrentWeapon == Weapons[CurrentMeleeWeapon] ? Weapons[CurrentRangeWeapon] : Weapons[CurrentMeleeWeapon]);

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

void UDDWeaponSystemComponent::WeaponSubSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("CanMeleeSubSkill %s"), CanMeleeSubSkill() ? TEXT("true") : TEXT("False"));
	if (CurrentWeapon == Weapons[CurrentRangeWeapon])
	{
		return;
	}

	if (CurrentWeapon == Weapons[CurrentMeleeWeapon] && CanMeleeSubSkill())
	{
		ParentSkeletal->GetAnimInstance()->Montage_Play(CurrentWeapon->GetSkillWeaponMontage());
		CurrentWeapon->SubSkill();
		
		UE_LOG(LogTemp, Warning, TEXT("Use Skill : %s"), *CurrentWeapon->GetFName().ToString());
	}

}

void UDDWeaponSystemComponent::WeaponStartAiming()
{
	if (CurrentWeapon == Weapons[CurrentMeleeWeapon])
	{	
		return;
	}

	if (CurrentWeapon == Weapons[CurrentRangeWeapon] && CanRangeAiming())
	{
		if (OnSetAimingDelegate.IsBound())
		{
			OnSetAimingDelegate.Execute(true);

		}
	}

}

void UDDWeaponSystemComponent::WeaponEndAiming()
{
	if (CurrentWeapon == Weapons[CurrentMeleeWeapon])
	{
		return;
	}

	if (CurrentWeapon == Weapons[CurrentRangeWeapon] && CanRangeAiming())
	{
		if (OnSetAimingDelegate.IsBound())
		{
			OnSetAimingDelegate.Execute(false);

		}
	}
}

//���� ����
bool UDDWeaponSystemComponent::IsUnequipMontage(const UAnimMontage* Montage) const
{
	return CurrentWeapon->GetUnequipWeaponMontage() == Montage;
}

bool UDDWeaponSystemComponent::CanMeleeSubSkill()
{
	//Skill ��� �� �Ұ���
	//Weapon Change �� �Ұ���
	//���� �� �Ұ���
	//�׾����� �� �Ұ���
	if (PlayerCharacter->GetCharacterMovement()->IsFalling())
	{
		return false;
	} 

	if (ParentSkeletal->GetAnimInstance()->IsAnyMontagePlaying())
	{
		if (!(ParentSkeletal->GetAnimInstance()->Montage_IsPlaying(CurrentWeapon->GetAttackMontage())))
		{
			return false;
		}
		
	}
	return true;
	
}

bool UDDWeaponSystemComponent::CanRangeAiming()
{
	//ĳ���� ��ų ����� �Ұ���
	//Weapon Change �� �Ұ���
	//�׾����� �� �Ұ���

	return true;
}



