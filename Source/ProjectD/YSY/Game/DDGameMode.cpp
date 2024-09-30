// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameMode.h"
#include "YSY/Game/DDPlayerState.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDBuildingManager.h"
#include "YSY/Manager/DDWaveManager.h"
#include "YSY/Game/DDPlayerController.h"

ADDGameMode::ADDGameMode()
{
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassRef(TEXT("/Script/ProjectD.DDHUD"));

	if (HUDClassRef.Class)
	{
		HUDClass = HUDClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ProjectD.DDPlayerController"));

	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateClassRef(TEXT("/Script/ProjectD.DDGameState"));

	if (GameStateClassRef.Class)
	{
		GameStateClass = GameStateClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateClassRef(TEXT("/Script/ProjectD.DDPlayerState"));

	if (PlayerStateClassRef.Class)
	{
		PlayerStateClass = PlayerStateClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/YSY/LevelStreamingTest/Widget/YSY_WBP_MyLoading.YSY_WBP_MyLoading_C'"));

	if (LoadingWidgetClassRef.Class)
	{
		LoadingWidgetClass = LoadingWidgetClassRef.Class;
	}

	LoadingWidget = CreateWidget(GetWorld(), LoadingWidgetClass);
}

void ADDGameMode::InitializePoolAsync(TFunction<void()> OnComplete)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, OnComplete]()
		{
			auto TempGameInstance = Cast<UDDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			TempGameInstance->GetWaveManager()->InitStage(CurrentStage);
			TempGameInstance->GetBuildingManager()->HandleBuildingPoolsOnLevelChange();

			AsyncTask(ENamedThreads::GameThread, OnComplete);
		});
}

void ADDGameMode::StageStart(const int32& NewCurrentStage)
{
	CurrentStage = NewCurrentStage;
	InitializePoolAsync([this]()
		{
			if (LoadingWidget)
			{
				LoadingWidget->RemoveFromParent();
			}
			auto TempPlayerController = Cast<ADDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			TempPlayerController->ShowMainWidget();
			FInputModeGameOnly InputModeGameOnly;
			TempPlayerController->SetInputMode(InputModeGameOnly);
			TempPlayerController->SetShowMouseCursor(false);
			TempPlayerController->SetIgnoreMoveInput(false);
		});
}
