
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDWeaponData.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Melee, // TODO : LJW Need to write UMETA
	Range,
	Unknown
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
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString WeaponInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 BuildCost;

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
};