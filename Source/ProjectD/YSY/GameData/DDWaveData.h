#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DDWaveData.generated.h"

USTRUCT(BlueprintType)
struct FDDWaveData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDDWaveData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 WaveNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TArray<int32> EnemyOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TArray<int32> PathOrder;
};