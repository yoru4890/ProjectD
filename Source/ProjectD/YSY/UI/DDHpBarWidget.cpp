// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/UI/DDHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "YSY/Interface/DDCharacterWidgetInterface.h"

UDDHpBarWidget::UDDHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UDDHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EnemyHpBar")));
	OwnerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("EnemyName")));
	ensure(HpProgressBar);
	ensure(OwnerName);

	IDDCharacterWidgetInterface* CharacterWidget = Cast<IDDCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

	OwnerName->SetVisibility(ESlateVisibility::Hidden);
	HpProgressBar->SetPercent(CurrentHp / MaxHp);
	SetVisiblePorgressBar(false);
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

void UDDHpBarWidget::SetVisiblePorgressBar(bool bIsVisible)
{
	auto NewVisible = bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	HpProgressBar->SetVisibility(NewVisible);
	//OwnerName->SetVisibility(NewVisible);
}

void UDDHpBarWidget::SetOwnerName(const FName& EnemyName)
{
	OwnerName->SetText(FText::FromName(EnemyName));
}
