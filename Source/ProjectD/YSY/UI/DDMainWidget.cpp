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

void UDDMainWidget::SetRifleAmmoText(int32 CurrentLoadedAmmo, int32 CurrentUnLoadedAmmo)
{
	FString AmmoString = FString::Printf(TEXT("%d / %d"), CurrentLoadedAmmo, CurrentUnLoadedAmmo);
	RifleAmmoText->SetText(FText::FromString(AmmoString));
	UE_LOG(LogTemp, Warning, TEXT("SetRifleAmmoText : %d, %d"), CurrentLoadedAmmo, CurrentUnLoadedAmmo);
}

void UDDMainWidget::GetRifleAmmoFromText(int32& OutLoadedAmmo, int32& OutUnLoadedAmmo)
{
	FString AmmoString = RifleAmmoText->GetText().ToString();
	FString LoadedAmmoString;
	FString UnLoadedAmmoString;

	if (AmmoString.Split(TEXT(" / "), &LoadedAmmoString, &UnLoadedAmmoString))
	{
		OutLoadedAmmo = FCString::Atoi(*LoadedAmmoString);
		OutUnLoadedAmmo = FCString::Atoi(*UnLoadedAmmoString);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to parse Rifle Ammo text"));
		OutLoadedAmmo = 0;
		OutUnLoadedAmmo = 0;
	}
}

void UDDMainWidget::SetLoadedRifleAmmoText(int32 CurrentLoadedAmmo)
{
	int32 UnLoadedAmmo;
	GetRifleAmmoFromText(UnLoadedAmmo, UnLoadedAmmo); // 기존 값을 불러옴
	SetRifleAmmoText(CurrentLoadedAmmo, UnLoadedAmmo); // 새로운 값을 반영하여 텍스트 설정
}

void UDDMainWidget::SetUnLoadedRifleAmmoText(int32 CurrentUnLoadedAmmo)
{
	int32 LoadedAmmo;
	GetRifleAmmoFromText(LoadedAmmo, LoadedAmmo); // 기존 값을 불러옴
	SetRifleAmmoText(LoadedAmmo, CurrentUnLoadedAmmo); // 새로운 값을 반영하여 텍스트 설정
}

void UDDMainWidget::SetRemainingLivesText(int32 CurrentLives)
{
	FString RemainingLivesString = FString::Printf(TEXT("%d"), CurrentLives);

	RemainingLivesText->SetText(FText::FromString(RemainingLivesString));
}

void UDDMainWidget::SetVisibilityAmmoText(bool IsVisible)
{
	UE_LOG(LogTemp, Warning, TEXT("SetVisibilityAmmoText Called"));
	if (IsVisible)
	{
		RifleAmmoText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		RifleAmmoText->SetVisibility(ESlateVisibility::Hidden);
	}
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
	RifleAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RifleAmmoText")));
}
