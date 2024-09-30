
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FDDCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDDCharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f), SprintSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	 float Attack;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	 float AttackRange;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	 float AttackSpeed;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	 float MovementSpeed;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	 float SprintSpeed;
};