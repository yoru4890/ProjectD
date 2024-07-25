
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDMeleeData.generated.h"

UENUM(BlueprintType)
enum class EMeleeType : uint8
{
	Cudgel,
	Lightsaber,
	Sword,
	Unknown
};

USTRUCT(BlueprintType)
struct FDDMeleeData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDDMeleeData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EMeleeType MeleeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString MeleeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USkeletalMeshComponent* MeleeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString MeleeInformation;

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