
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "YSY/GameData/DDEffectData.h"
#include "DDEnemyData.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	LightArmor UMETA(DisplayName = "LightArmor"),
	Mechanical UMETA(DisplayName = "Mechanical"),
	Cybernetic UMETA(DisplayName = "Cybernetic"),

	Unknown
};

UENUM(BlueprintType)
enum class EEnemyAttackType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),

	Unknown
};

USTRUCT(BlueprintType)
struct FDDEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDDEnemyData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FName EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TArray<FName> WeakPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EEnemyAttackType EnemyAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float ScaleSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float CapsuleRadiusSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float CapsuleHalfHeightSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MeshLocationZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AggroRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float HealthWidgetHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 GoldDropAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	uint8 bIsBoss : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	uint8 bIsElite : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Data)
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Data)
	TSoftObjectPtr<UAnimBlueprint> AnimationBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Data)
	TSoftObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Data)
	TArray<FEffectData> AttackEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Data)
	TArray<FEffectData> DeathEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<USoundBase> AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float SoundStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float VolumeMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float PitchMultiplier{ 1.0f };
};