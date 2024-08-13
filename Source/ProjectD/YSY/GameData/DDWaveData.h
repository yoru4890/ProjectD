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
	TArray<int32> EnemyCountsPerWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TArray<FString> EnemyOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TArray<FString> GateOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TArray<int32> GatePathMappings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TMap<FName, int32> EnemyPoolSizes;
};