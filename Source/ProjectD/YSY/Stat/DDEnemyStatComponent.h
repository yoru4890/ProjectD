// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDEnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementSpeedChangeSignature, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTD_API UDDEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDDEnemyStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnMovementSpeedChangeSignature OnMovementSpeedChange;

	FORCEINLINE float GetCurrentHp() const noexcept { return CurrentHp; }
	void SetCurrentHp(float NewHp);

	FORCEINLINE float GetMaxHp() const noexcept { return MaxHp; }
	void SetMaxHp(float NewHp) { MaxHp = NewHp; }

	FORCEINLINE float GetMovementSpeed() const noexcept { return MovementSpeedRate; }
	FORCEINLINE float GetDamageReceiveRate() const noexcept { return DamageReceiveRate; }

	float ApplyStatDamage(float InDamage);

	void ApplySlow(FTimerHandle& TimerHandle, float Time, float Amount);
	void ApplyFast(FTimerHandle& TimerHandle, float Time, float Amount);
	void UpdateMovementSpeed();

	void ApplyDamageReceiveIncrease(FTimerHandle& TimerHandle, float Time, float Amount);
	void ApplyDamageReceiveDecrease(FTimerHandle& TimerHandle, float Time, float Amount);
	void UpdateDamageReceive();

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float CurrentHp{};

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float MaxHp{};

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float MovementSpeedRate = 1.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float MovementSlowRate = 1.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float MovementFastRate = 1.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float DamageReceiveIncreaseRate = 1.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float DamageReceiveDecreaseRate = 1.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float DamageReceiveRate = 1.0f;
};
