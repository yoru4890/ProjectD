// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/UI/DDMainWidget.h"
#include "Components/TextBlock.h"

void UDDMainWidget::SetWaveText(int32 CurrentWave, int32 MaxWave)
{
	FString WaveString = FString::Printf(TEXT("%d / %d"), CurrentWave, MaxWave);

	WaveText->SetText(FText::FromString(WaveString));
}

void UDDMainWidget::SetGoldText(int32 CurrentGold)
{
	// 숫자를 쉼표로 구분된 형식으로 변환
	FText GoldFormattedText = FText::AsNumber(CurrentGold);

	if (GoldText)
	{
		// 텍스트 위젯에 숫자 설정
		GoldText->SetText(GoldFormattedText);
	}

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

void UDDMainWidget::SetWeaponSlotActive(int32 SlotIndex)
{
	FName FunctionName;

	switch (SlotIndex)
	{
	case 1:
		FunctionName = TEXT("Active1");
		break;
	case 2:
		FunctionName = TEXT("Active2");
		break;
	case 3:
		FunctionName = TEXT("Active3");
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid SlotIndex: %d"), SlotIndex);
		return; // 잘못된 슬롯 인덱스이면 종료
	}

	// FindFunction을 사용하여 해당 함수가 있는지 확인
	UFunction* Function = WeaponSlot->FindFunction(FunctionName);

	if (Function)
	{
		// 필요한 파라미터가 있을 경우, 준비
		struct FCustomParams
		{
			// 여기에 함수에 맞는 파라미터 정의
		};

		FCustomParams Params;  // 파라미터 초기화
		WeaponSlot->ProcessEvent(Function, &Params);  // 함수 호출
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function %s not found on Widget"), *FunctionName.ToString());
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
