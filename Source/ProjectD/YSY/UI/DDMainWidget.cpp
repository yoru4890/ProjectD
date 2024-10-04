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
	// ���ڸ� ��ǥ�� ���е� �������� ��ȯ
	FText GoldFormattedText = FText::AsNumber(CurrentGold);

	if (GoldText)
	{
		// �ؽ�Ʈ ������ ���� ����
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
	GetRifleAmmoFromText(UnLoadedAmmo, UnLoadedAmmo); // ���� ���� �ҷ���
	SetRifleAmmoText(CurrentLoadedAmmo, UnLoadedAmmo); // ���ο� ���� �ݿ��Ͽ� �ؽ�Ʈ ����
}

void UDDMainWidget::SetUnLoadedRifleAmmoText(int32 CurrentUnLoadedAmmo)
{
	int32 LoadedAmmo;
	GetRifleAmmoFromText(LoadedAmmo, LoadedAmmo); // ���� ���� �ҷ���
	SetRifleAmmoText(LoadedAmmo, CurrentUnLoadedAmmo); // ���ο� ���� �ݿ��Ͽ� �ؽ�Ʈ ����
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
		return; // �߸��� ���� �ε����̸� ����
	}

	// FindFunction�� ����Ͽ� �ش� �Լ��� �ִ��� Ȯ��
	UFunction* Function = WeaponSlot->FindFunction(FunctionName);

	if (Function)
	{
		// �ʿ��� �Ķ���Ͱ� ���� ���, �غ�
		struct FCustomParams
		{
			// ���⿡ �Լ��� �´� �Ķ���� ����
		};

		FCustomParams Params;  // �Ķ���� �ʱ�ȭ
		WeaponSlot->ProcessEvent(Function, &Params);  // �Լ� ȣ��
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
