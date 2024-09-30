#pragma once

#include "Coreminimal.h"
#include "DDEffectData.generated.h"

USTRUCT(BlueprintType)
struct FEffectData
{
	GENERATED_BODY()

public:
	FEffectData() : SoundStartTime(0.0f) {}

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float VolumeMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float PitchMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float EffectScale{ 1.0f };
};