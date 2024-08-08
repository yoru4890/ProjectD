
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDEnemyData.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	LightArmor UMETA(DisplayName = "LightArmor"),
	Mechanical UMETA(DisplayName = "Mechanical"),
	Cybernetic UMETA(DisplayName = "Cybernetic"),

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
	FName WeakPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EEnemyType EnemyType;

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
	FString MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Data)
	FString AnimationBlueprintPath;
};