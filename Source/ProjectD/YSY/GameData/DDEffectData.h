#pragma once

#include "Coreminimal.h"
#include "DDEffectData.generated.h"

USTRUCT(BlueprintType)
struct FEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<UParticleSystem>  CascadeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UNiagaraSystem> NiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<USoundBase> SoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TArray<FName> LocationNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float SoundStartTime;
};