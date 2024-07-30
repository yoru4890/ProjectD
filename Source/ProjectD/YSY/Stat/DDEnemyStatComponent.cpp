// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Stat/DDEnemyStatComponent.h"

UDDEnemyStatComponent::UDDEnemyStatComponent()
{
	bWantsInitializeComponent = true;
}

void UDDEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetCurrentHp(100.0f); // TODO : YSY Setting HpData
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

void UDDEnemyStatComponent::ApplySlow(float Amount)
{
	MovementSlowRate = Amount;
	UpdateMovementSpeed();
}

void UDDEnemyStatComponent::ApplyFast(float Amount)
{
	MovementFastRate = Amount;
	UpdateMovementSpeed();
}

// Need to bind : FOnMovementSpeedChangeSignature OnMovementSpeedChange bind to OwnerActor
void UDDEnemyStatComponent::UpdateMovementSpeed()
{
	MovementSpeedRate *= (MovementSlowRate * MovementFastRate);
	OnMovementSpeedChange.Broadcast(MovementSpeedRate);
}

void UDDEnemyStatComponent::ApplyDamageReceiveIncrease(float Amount)
{
	DamageReceiveIncreaseRate = Amount;
	UpdateDamageReceive();
}

void UDDEnemyStatComponent::ApplyDamageReceiveDecrease(float Amount)
{
	DamageReceiveDecreaseRate = Amount;
	UpdateDamageReceive();
}

void UDDEnemyStatComponent::UpdateDamageReceive()
{
	DamageReceiveRate *= (DamageReceiveDecreaseRate * DamageReceiveIncreaseRate);
}