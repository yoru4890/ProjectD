// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/UI/DDHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "YSY/Interface/DDCharacterWidgetInterface.h"

UDDHpBarWidget::UDDHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UDDHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("")));
	ensure(HpProgressBar);

	IDDCharacterWidgetInterface* CharacterWidget = Cast<IDDCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UDDHpBarWidget::UpdateStat(float InMaxHp)
{
	MaxHp = InMaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}
}

void UDDHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}
}
