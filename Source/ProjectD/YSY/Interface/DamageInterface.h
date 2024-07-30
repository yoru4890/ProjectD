// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EDotDamageType : uint8
{
	Acid UMETA(DisplayName = "Acid"),
	Fire UMETA(DisplayName = "Fire")
};

UENUM(BlueprintType)
enum class EDebuffType : uint8
{
	Slow UMETA(DisplayName = "Slow"),
	Stun UMETA(DisplayName = "Stun"),
	DamageIncrease UMETA(DisplayName = "DamageIncrease")
};

class PROJECTD_API IDamageInterface
{
	GENERATED_BODY()

public:

	virtual float ApplyDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) = 0;
	virtual void ApplyDamageOverTime(EDotDamageType DamageType, float Time, float TimeInterval, float DamageAmount) = 0;
	virtual void ApplyChainDamage(int DamageAmount, int NumberOfChain) = 0;
	virtual void ApplyDebuff(EDebuffType DebuffType, float Time, float DebuffRate) = 0;
};
