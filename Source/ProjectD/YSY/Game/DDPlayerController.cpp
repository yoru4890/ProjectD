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

ADDPlayerController::ADDPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/YSY/Widget/YSY_WBP_Main.YSY_WBP_Main_C'"));

	if (MainWidgetClassRef.Class)
	{
		MainWidgetClass = MainWidgetClassRef.Class;
	}
}

void ADDPlayerController::ShowMainWidget()
{
	MainWidget = CreateWidget(GetWorld(), MainWidgetClass);
	auto PlayerWidgetInterface = Cast<IDDCharacterWidgetInterface>(GetPawn());
	ensure(PlayerWidgetInterface);
	if (PlayerWidgetInterface)
	{
		auto TempMainWidget = Cast<UDDMainWidget>(MainWidget);
		PlayerWidgetInterface->SetupCharacterWidget(TempMainWidget->GetHpBarWidget());

		auto TempPlayerState = Cast<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		TempPlayerState->OnGoldChanged.AddDynamic(TempMainWidget, &UDDMainWidget::SetGoldText);
		TempMainWidget->SetGoldText(TempPlayerState->GetGold());

		auto TempGameInstance = Cast<UDDGameInstance>(GetGameInstance());

		auto TempWaveManager = TempGameInstance->GetWaveManager();

		TempWaveManager->OnRemainingLivesChanged.AddDynamic(TempMainWidget, &UDDMainWidget::SetRemainingLivesText);

		TempMainWidget->SetRemainingLivesText(TempWaveManager->GetRemainingLives());

		TempWaveManager->OnWaveChangedSignature.AddDynamic(TempMainWidget, &UDDMainWidget::SetWaveText);

		TempMainWidget->SetWaveText(TempWaveManager->GetCurrentWave(), TempWaveManager->GetMaxWave());
	}
	MainWidget->AddToViewport();
	
}

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	
	
}
