// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LJW/Weapon/DDWeaponData.h"
#include "DDWeaponBase.generated.h"



UCLASS()
class PROJECTD_API ADDWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDWeaponBase();

public:

	void InitData(const FName& RowName, const FDDWeaponData& WeaponData);

	void DisableWeapon();
	void EnableWeapon();

	//Equip, Unequip GET/SET

	FORCEINLINE UAnimMontage* GetEquipWeaponMontage() const noexcept{ return EquipWeaponAnim; }
	void SetEquipWeaponMontage(UAnimMontage* InMontage) { EquipWeaponAnim = InMontage; }

	FORCEINLINE UAnimMontage* GetUnequipWeaponMontage() const noexcept { return UnequipWeaponAnim; }
	void SetUnequipWeaponMontage(UAnimMontage* InMontage) { UnequipWeaponAnim = InMontage; }

	FORCEINLINE UAnimMontage* GetSkillWeaponMontage() const noexcept { return SkillWeaponAnim; }
	void SetSkillWeaponMontage(UAnimMontage* InMontage) { SkillWeaponAnim = InMontage; }

	FORCEINLINE UAnimMontage* GetAttackMontage() const noexcept { return AttackAnim; }
	void SetAttackMontage(UAnimMontage* InMontage) { AttackAnim = InMontage; }

	FORCEINLINE FName GetSocketName() const noexcept { return WeaponSocketName; }

	
	virtual void SubSkill() {};
	virtual void Attack() {};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletal;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName WeaponRowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString WeaponDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FString WeaponInformation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 UnlockLP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float AttackCooltime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UAnimMontage> EquipWeaponAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UAnimMontage> UnequipWeaponAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UAnimMontage> SkillWeaponAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UAnimMontage> AttackAnim;
	
};
