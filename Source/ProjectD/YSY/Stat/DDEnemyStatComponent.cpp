// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Stat/DDEnemyStatComponent.h"

UDDEnemyStatComponent::UDDEnemyStatComponent()
{
	bWantsInitializeComponent = true;
}

void UDDEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UDDEnemyStatComponent::SetCurrentHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}

float UDDEnemyStatComponent::ApplyStatDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetCurrentHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UDDEnemyStatComponent::ApplySlow(FTimerHandle& TimerHandle, float Time, float Amount)
{
	MovementSlowRate = Amount;
	UpdateMovementSpeed();
	UE_LOG(LogTemp, Warning, TEXT("SlowStart%f"), Amount);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			MovementSlowRate = 0.f;
			UpdateMovementSpeed();
			UE_LOG(LogTemp, Warning, TEXT("SlowEnd"));
		}
	, 0.01f, false, Time);
}

void UDDEnemyStatComponent::ApplyFast(FTimerHandle& TimerHandle, float Time, float Amount)
{
	MovementFastRate = Amount;
	UpdateMovementSpeed();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			MovementFastRate = 1.0f;
			UpdateMovementSpeed();
		}
	, 0.01f, false, Time);
}

// Need to bind : FOnMovementSpeedChangeSignature OnMovementSpeedChange bind to OwnerActor
void UDDEnemyStatComponent::UpdateMovementSpeed()
{
	MovementSpeedRate = (1 - MovementSlowRate) * (MovementFastRate);
	OnMovementSpeedChange.Broadcast(MovementSpeedRate);
}

// How to use : 50% -> Amount = 1.5f
void UDDEnemyStatComponent::ApplyDamageReceiveIncrease(FTimerHandle& TimerHandle, float Time, float Amount)
{
	DamageReceiveIncreaseRate = Amount;
	UpdateDamageReceive();
	UE_LOG(LogTemp, Warning, TEXT("DamageReceiveIncreaseStart%f"), Amount);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			DamageReceiveIncreaseRate = 0.f;
			UpdateDamageReceive();
			UE_LOG(LogTemp, Warning, TEXT("DamageReceiveIncreaseEnd"));
		}
	, 0.01f, false, Time);
}

// How to use : 20% -> Amount = 0.8f
void UDDEnemyStatComponent::ApplyDamageReceiveDecrease(FTimerHandle& TimerHandle, float Time, float Amount)
{
	DamageReceiveDecreaseRate = Amount;
	UpdateDamageReceive();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			DamageReceiveDecreaseRate = 1.0f;
			UpdateDamageReceive();
		}
	, 0.01f, false, Time);
}

void UDDEnemyStatComponent::UpdateDamageReceive()
{
	DamageReceiveRate = (1+DamageReceiveIncreaseRate)*(DamageReceiveDecreaseRate);
}