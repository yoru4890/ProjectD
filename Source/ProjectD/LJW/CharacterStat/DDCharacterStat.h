
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FDDCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDDCharacterStat() {}

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
};