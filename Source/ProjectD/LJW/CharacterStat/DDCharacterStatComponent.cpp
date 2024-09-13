// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/CharacterStat/DDCharacterStatComponent.h"

// Sets default values for this component's properties
UDDCharacterStatComponent::UDDCharacterStatComponent()
{
	// 이걸 true해야 InitializeComponent 함수가 호출됨.
	bWantsInitializeComponent = true;
}

void UDDCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	//SetHp(BaseStat.MaxHp);
}

float UDDCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) 
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UDDCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = NewHp;
	//CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentHp);
}





