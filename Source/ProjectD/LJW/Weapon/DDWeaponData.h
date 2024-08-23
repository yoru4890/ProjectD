
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDWeaponData.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),
	Unknown UMETA(DisplayName = "Unknown")
};


USTRUCT(BlueprintType)
struct FDDWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDDWeaponData() {}
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class ADDWeaponBase> WeaponClass; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString WeaponName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString WeaponInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 UnlockLP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float AttackCooltime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<UAnimMontage> UnequipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<UAnimMontage> SkillMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<UAnimMontage> AttackMontage;

};