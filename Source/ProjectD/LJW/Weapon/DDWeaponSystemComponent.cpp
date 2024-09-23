// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponSystemComponent.h"
#include "YSY/Game/DDDataManager.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "LJW/Weapon/DDWeaponCudgel.h"
#include "LJW/Weapon//DDWeaponRifle.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "LJW/Interface/CameraFOVInterface.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Game/DDPlayerState.h"



// Sets default values for this component's properties
UDDWeaponSystemComponent::UDDWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Rifle Zoom Timeline Curve
	static ConstructorHelpers::FObjectFinder<UCurveFloat>ZoomCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/0000/LJW/Blueprints/Data/FloatCurve_RifleZoom.FloatCurve_RifleZoom'"));
	if (nullptr != ZoomCurveRef.Object)
	{
		ZoomCurve = ZoomCurveRef.Object;
	}

	
}

void UDDWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeWeapon();
	InitTimeline();
	ComboSectionIndex = FName("A");
}

void UDDWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RifleZoomTL.TickTimeline(DeltaTime);
}

void UDDWeaponSystemComponent::InitializeWeapon()
{
	// 싱글톤에서 data들을 얻음, 데이터 순서 중요
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	auto WeaponDatas = MyGameInstance->GetDataManager()->GetWeaponDataTable();

	//Player에 무기 장착
	if (GetOwner())
	{
		//FindComponentByClass -> 여러 개일 경우 가장 첫 번째 메쉬 반환
		ParentSkeletal = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		PlayerCharacter = Cast<ACharacter>(GetOwner());
		PlayerAnimInstance = ParentSkeletal->GetAnimInstance();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is null"));
	}

	for (auto& [WeaponDataName, WeaponDataValue] : WeaponDatas)
	{
		UClass* TempClass = WeaponDataValue.WeaponClass;
		check(TempClass);

		//무기 클래스 스폰
		auto TempWeapon = GetWorld()->SpawnActor<ADDWeaponBase>(TempClass, FVector::ZeroVector, FRotator::ZeroRotator);
		check(TempWeapon);
		UE_LOG(LogTemp, Warning, TEXT("Spawn Weapon : %s"), *WeaponDataName.ToString());

		
		//무기 Data
		TempWeapon->InitData(WeaponDataName,WeaponDataValue);
		Weapons.Emplace(TempWeapon);
		TempWeapon->AttachToComponent(ParentSkeletal, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TempWeapon->GetSocketName());
		
	}

	//Initializing current weapon
	CurrentMeleeWeapon = static_cast<int32>(EWeaponType::Cudgel);
	CurrentRangeWeapon = static_cast<int32>(EWeaponType::Rifle);
	CurrentWeapon = Weapons[CurrentMeleeWeapon];
	CurrentWeapon->EnableWeapon();

	//Init Cudgel Collision
	CudgelCollision = Cast<ADDWeaponCudgel>(CurrentWeapon)->CollisionCapsule;
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

		PlayerAnimInstance->Montage_Play(CurrentWeapon->GetEquipWeaponMontage());
	}
}

void UDDWeaponSystemComponent::PlayUnequipMontage()
{
	if (CurrentWeapon->GetUnequipWeaponMontage())
	{
		PlayerAnimInstance->Montage_Play(CurrentWeapon->GetUnequipWeaponMontage());
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
		PlayerAnimInstance->Montage_Play(CurrentWeapon->GetSkillWeaponMontage());
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
		if (OnGetAimingDelegate.Execute())
		{
			return;
		}

		if (OnSetAimingDelegate.IsBound())
		{
			OnSetAimingDelegate.Execute(true);
			RifleZoomTL.Play();

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
			//Attack Montage가 Aim Animation을 덮어 씌우는 현상 해결
			OnSetAimingDelegate.Execute(false);
			if (PlayerAnimInstance->Montage_IsPlaying(CurrentWeapon->GetAttackMontage()))
			{
				PlayerAnimInstance->Montage_Stop(0.25f);
			}
			RifleZoomTL.Reverse();
		}
	}
}

void UDDWeaponSystemComponent::WeaponAttack()
{
	//Melee
	if (CurrentWeapon == Weapons[CurrentMeleeWeapon] && CanAttacking())
	{
		if (PlayerAnimInstance->Montage_IsPlaying(CurrentWeapon->GetAttackMontage()))
		{
			if (PlayerAnimInstance->Montage_GetCurrentSection() == ComboSectionIndex || ComboSectionIndex == FName("A"))
			{
				return;
			}
			PlayerAnimInstance->Montage_JumpToSection(ComboSectionIndex, CurrentWeapon->GetAttackMontage());
			CurrentWeapon->Attack();
		}
		else
		{
			PlayerAnimInstance->Montage_Play(CurrentWeapon->GetAttackMontage());
		}
		
	}

	//Range
	if (CurrentWeapon == Weapons[CurrentRangeWeapon])
	{
		//Aim 중 일 때만 가능
		if (OnGetAimingDelegate.Execute())
		{
			ADDWeaponRifle* WeaponRifle = Cast<ADDWeaponRifle>(CurrentWeapon);
			if (WeaponRifle->SubtractLoadedRifleAmmo(1))
			{
				PlayerAnimInstance->Montage_Play(CurrentWeapon->GetAttackMontage());
				CurrentWeapon->Attack();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Reload Cast Error"));
			}
		}
	}
	

}

void UDDWeaponSystemComponent::ReloadWeapon()
{
	if (CurrentWeapon == Weapons[CurrentRangeWeapon] && CanAttacking())
	{
		ADDWeaponRifle* WeaponRifle = Cast<ADDWeaponRifle>(CurrentWeapon);
		if (WeaponRifle->Reload())
		{
			PlayerAnimInstance->Montage_Play(WeaponRifle->GetReloadMontage());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Reload Cast Error"));
		}
	}
}

//조건 모음
bool UDDWeaponSystemComponent::IsUnequipMontage(const UAnimMontage* Montage) const
{
	return CurrentWeapon->GetUnequipWeaponMontage() == Montage;
}

bool UDDWeaponSystemComponent::CanMeleeSubSkill()
{
	//Skill 사용 중 불가능
	//Weapon Change 중 불가능
	//점프 중 불가능
	//죽어있을 때 불가능
	if (PlayerCharacter->GetCharacterMovement()->IsFalling())
	{
		return false;
	} 

	if (PlayerAnimInstance->IsAnyMontagePlaying())
	{
		if (!(PlayerAnimInstance->Montage_IsPlaying(CurrentWeapon->GetAttackMontage())))
		{
			return false;
		}
		
	}
	return true;
	
}

bool UDDWeaponSystemComponent::CanRangeAiming()
{
	//스킬 사용중 불가능
	//Weapon Change 중 불가능
	//죽어있을 때 불가능
	//Attack 중에 가능
	if (PlayerAnimInstance->IsAnyMontagePlaying() && !bIsOnTimeline)
	{
		if (!(PlayerAnimInstance->Montage_IsPlaying(CurrentWeapon->GetAttackMontage())))
		{
			return false;
		}

	}
	return true;
}

bool UDDWeaponSystemComponent::CanAttacking()
{
	//스킬 사용 중에 불가능
	//Weapon Change 중 불가능
	if (PlayerAnimInstance->IsAnyMontagePlaying())
	{
		if (!PlayerAnimInstance->Montage_IsPlaying(CurrentWeapon->GetAttackMontage()))
		{
			return false;
		}
	}
	return true;
}

void UDDWeaponSystemComponent::UpdateRifleZoom()
{
	bIsOnTimeline = true;
	float CurveTime = RifleZoomTL.GetPlaybackPosition();
	float CurveValue = ZoomCurve->GetFloatValue(CurveTime);
	float InFieldOfViewValue = FMath::Lerp(90.0f, 70.0f, CurveValue);

	ICameraFOVInterface* CameraFOVInterface = Cast<ICameraFOVInterface>(GetOwner());
	CameraFOVInterface->SetCameraFOV(InFieldOfViewValue);
}

void UDDWeaponSystemComponent::FinishRifleZoom()
{
	bIsOnTimeline = false;
}

void UDDWeaponSystemComponent::InitTimeline()
{
	FOnTimelineFloat OnTimelineFloatDelegate;
	FOnTimelineEventStatic OnTimelineEventDelegate;

	OnTimelineFloatDelegate.BindUFunction(this, FName("UpdateRifleZoom"));
	OnTimelineEventDelegate.BindUObject(this, &UDDWeaponSystemComponent::FinishRifleZoom);
	
	RifleZoomTL.AddInterpFloat(ZoomCurve, OnTimelineFloatDelegate);
	RifleZoomTL.SetTimelineFinishedFunc(OnTimelineEventDelegate);

	bIsOnTimeline = false;
}

ADDWeaponBase* UDDWeaponSystemComponent::GetCurrentRangeWeaponInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapons Num : %d"),Weapons.Num());
	UE_LOG(LogTemp, Warning, TEXT("CurrentRangeWeapon : %d"), CurrentRangeWeapon);
	if (Weapons.IsValidIndex(CurrentRangeWeapon))
	{
		return Weapons[CurrentRangeWeapon];
	}
	UE_LOG(LogTemp, Warning, TEXT("GetCurrentRangeWeaponInstance error"));
	return nullptr;
}



