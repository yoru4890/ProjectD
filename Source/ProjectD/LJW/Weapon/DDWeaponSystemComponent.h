// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "Components/TimeLineComponent.h"
#include "DDWeaponSystemComponent.generated.h"

DECLARE_DELEGATE_RetVal(bool, FOnGetIsAimingSignature);
DECLARE_DELEGATE_OneParam(FOnSetIsAimingSignature, bool);
DECLARE_DELEGATE_RetVal(bool, FOnGetIsDeadSignature);
DECLARE_DELEGATE_OneParam(FOnSetIsDeadSignature, bool);
DECLARE_DELEGATE_OneParam(FOnSetWeaponIndexSignature, int32);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Cudgel UMETA(DisplayName = "Cudgel"),
	LightSaber UMETA(DisplayName = "LightSaber"),
	Sword UMETA(DisplayName = "Sword"),

	Rifle UMETA(DisplayName = "Rifle"),
	Sniper UMETA(DisplayName = "Sniper"),
	Bazooka UMETA(DisplayName = "Bazooka"),
	Machinegun UMETA(DisplayName = "Machinegun"),
	
 	Unknown UMETA(DisplayName = "Unknown")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UDDWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDDWeaponSystemComponent();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeWeapon();
	
	void EquipMeleeWeapon();
	void EquipRangeWeapon();

	void PlayEquipMontage();
	void PlayUnequipMontage();

	void WeaponSubSkill();
	void WeaponStartAiming();
	void WeaponEndAiming();
	
	void WeaponAttack();
	void ReloadWeapon();

	//조건 모음
	bool IsUnequipMontage(const UAnimMontage* Montage) const;
	bool CanMeleeSubSkill();
	bool CanRangeAiming();
	bool CanAttacking();

	UFUNCTION()
	void UpdateRifleZoom(); //Timeline Update(Tick)
	UFUNCTION()
	void FinishRifleZoom(); //Timeline Finish(End)
	UFUNCTION()
	void InitTimeline();

	ADDWeaponBase* GetCurrentRangeWeaponInstance();

public:
	FOnGetIsAimingSignature OnGetAimingDelegate;
	FOnSetIsAimingSignature OnSetAimingDelegate;
	FOnGetIsDeadSignature OnGetDeadDelegate;
	FOnSetIsDeadSignature OnSetDeadDelegate;
	FOnSetWeaponIndexSignature OnSetWeaponIndexDelegate;

	FName ComboSectionIndex;

	UPROPERTY()
	TObjectPtr<UCapsuleComponent> CudgelCollision;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TArray<ADDWeaponBase*> Weapons;

	//들고있는 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADDWeaponBase> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	int32 CurrentMeleeWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	int32 CurrentRangeWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Weapon, meta = (AllowPrivateAccess = "true"))
	EWeaponType CurrentWeaponEnum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ParentSkeletal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACharacter> PlayerCharacter;


	UPROPERTY()
	TObjectPtr<UAnimInstance> PlayerAnimInstance;

	FTimeline RifleZoomTL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ZoomCurve;

	uint32 bIsOnTimeline : 1;

	
}; 
