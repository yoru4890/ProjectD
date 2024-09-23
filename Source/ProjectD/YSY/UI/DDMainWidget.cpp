// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/UI/DDMainWidget.h"
#include "Components/TextBlock.h"

void UDDMainWidget::SetWaveText(int32 CurrentWave, int32 MaxWave)
{
	FString WaveString = FString::Printf(TEXT("Wave %d / %d"), CurrentWave, MaxWave);

	WaveText->SetText(FText::FromString(WaveString));
}

void UDDMainWidget::SetGoldText(int32 CurrentGold)
{
	FString GoldString = FString::Printf(TEXT("Gold : %d"), CurrentGold);

	GoldText->SetText(FText::FromString(GoldString));
}

void UDDMainWidget::SetLoadedRifleAmmoText(int32 CurrentLoadedAmmo)
{
	LoadedRifleAmmoText->SetText(FText::AsNumber(CurrentLoadedAmmo));
}

void UDDMainWidget::SetUnLoadedRifleAmmoText(int32 CurrentUnLoadedAmmo)
{
	UnLoadedRifleAmmoText->SetText(FText::AsNumber(CurrentUnLoadedAmmo));
}

void UDDMainWidget::SetRemainingLivesText(int32 CurrentLives)
{
	FString RemainingLivesString = FString::Printf(TEXT("%d"), CurrentLives);

	RemainingLivesText->SetText(FText::FromString(RemainingLivesString));
}

void UDDMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!PlayerHealth)
	{
		UE_LOG(LogTemp, Warning, TEXT("MainWidget hasn't PlayerHealth"))
	}

	WaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WaveText")));
	GoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoldText")));
	RemainingLivesText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RemainingLivesText")));
	LoadedRifleAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LoadedRifleAmmo")));
	UnLoadedRifleAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("UnLoadedRifleAmmo")));
}
