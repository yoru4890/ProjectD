// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/UI/DDPlayerHPBarWidget.h"
#include "Components/ProgressBar.h"

UDDPlayerHPBarWidget::UDDPlayerHPBarWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UDDPlayerHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HpProgressBar);

	HpProgressBar->SetPercent(CurrentHp / MaxHp);
}

void UDDPlayerHPBarWidget::UpdateStat(float InMaxHp)
{
	MaxHp = InMaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}
}

void UDDPlayerHPBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}
}
