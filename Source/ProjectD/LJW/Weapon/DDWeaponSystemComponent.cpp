// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Weapon/DDWeaponSystemComponent.h"
#include "YSY/Game/DDGameSingleton.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "LJW/Interface/CameraFOVInterface.h"



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
}

void UDDWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RifleZoomTL.TickTimeline(DeltaTime);
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
			ParentSkeletal = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
			PlayerCharacter = Cast<ACharacter>(GetOwner());
			
			//무기 Data
			TempWeapon->InitData(WeaponDataName,WeaponDataValue);
			Weapons.Emplace(TempWeapon);
			TempWeapon->AttachToComponent(ParentSkeletal, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TempWeapon->GetSocketName());
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is null"));
		}
		
		
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
		if (OnGetAimingDelegate.Execute())
		{
			return;
		}

		if (OnSetAimingDelegate.IsBound())
		{
			OnSetAimingDelegate.Execute(true);
			RifleZoomTL.PlayFromStart();
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
			RifleZoomTL.Reverse();
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
	//캐릭터 스킬 사용중 불가능
	//Weapon Change 중 불가능
	//죽어있을 때 불가능

	return true;
}

void UDDWeaponSystemComponent::UpdateRifleZoom()
{
	float CurveTime = RifleZoomTL.GetPlaybackPosition();
	float CurveValue = ZoomCurve->GetFloatValue(CurveTime);
	float InFieldOfViewValue = FMath::Lerp(90.0f, 70.0f, CurveValue);

	ICameraFOVInterface* CameraFOVInterface = Cast<ICameraFOVInterface>(GetOwner());
	CameraFOVInterface->SetCameraFOV(InFieldOfViewValue);
}

void UDDWeaponSystemComponent::FinishRifleZoom()
{

}

void UDDWeaponSystemComponent::InitTimeline()
{
	FOnTimelineFloat OnTimelineFloatDelegate;
	FOnTimelineEventStatic OnTimelineEventDelegate;

	OnTimelineFloatDelegate.BindUFunction(this, FName("UpdateRifleZoom"));
	OnTimelineEventDelegate.BindUObject(this, &UDDWeaponSystemComponent::FinishRifleZoom);
	
	RifleZoomTL.AddInterpFloat(ZoomCurve, OnTimelineFloatDelegate);
	RifleZoomTL.SetTimelineFinishedFunc(OnTimelineEventDelegate);

	
}



