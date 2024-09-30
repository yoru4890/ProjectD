// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "YSY/UI/DDMainWidget.h"
#include "YSY/Interface/DDCharacterWidgetInterface.h"
#include "YSY/UI/DDPlayerHPBarWidget.h"
#include "YSY/Game/DDPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Manager/DDWaveManager.h"
#include "LJW/Character/DDCharacterPlayer.h"
#include "LJW/Weapon/DDWeaponRifle.h"
#include "LJW/Weapon/DDWeaponSystemComponent.h"

ADDPlayerController::ADDPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/LSM/Widget/LSM_WBP_Main.LSM_WBP_Main_C'"));

	if (MainWidgetClassRef.Class)
	{
		MainWidgetClass = MainWidgetClassRef.Class;
	}

	MainWidget = CreateWidget(GetWorld(), MainWidgetClass);
}

void ADDPlayerController::ShowMainWidget()
{
	
	auto PlayerWidgetInterface = Cast<IDDCharacterWidgetInterface>(GetPawn());
	ADDCharacterPlayer* MyCharacter = Cast<ADDCharacterPlayer>(GetPawn());

	ensure(PlayerWidgetInterface);
	if (PlayerWidgetInterface)
	{
		auto TempMainWidget = Cast<UDDMainWidget>(MainWidget);
		PlayerWidgetInterface->SetupCharacterWidget(TempMainWidget->GetHpBarWidget());
		PlayerWidgetInterface->SetupRifleAmmoText(TempMainWidget);

		auto TempPlayerState = Cast<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		if (!TempPlayerState->OnGoldChanged.IsBound())
		{
			TempPlayerState->OnGoldChanged.AddDynamic(TempMainWidget, &UDDMainWidget::SetGoldText);
		}
		TempMainWidget->SetGoldText(TempPlayerState->GetGold());

		auto TempGameInstance = Cast<UDDGameInstance>(GetGameInstance());

		auto TempWaveManager = TempGameInstance->GetWaveManager();

		if (!TempWaveManager->OnRemainingLivesChanged.IsBound())
		{
			TempWaveManager->OnRemainingLivesChanged.AddDynamic(TempMainWidget, &UDDMainWidget::SetRemainingLivesText);
		}

		TempMainWidget->SetRemainingLivesText(TempWaveManager->GetRemainingLives());

		if (!TempWaveManager->OnWaveChangedSignature.IsBound())
		{
			TempWaveManager->OnWaveChangedSignature.AddDynamic(TempMainWidget, &UDDMainWidget::SetWaveText);
		}

		TempMainWidget->SetWaveText(TempWaveManager->GetCurrentWave(), TempWaveManager->GetMaxWave());

	}
	MainWidget->AddToViewport();
	
}

UUserWidget* ADDPlayerController::GetMainWidget()
{
	return MainWidget;
}

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	
	
}
