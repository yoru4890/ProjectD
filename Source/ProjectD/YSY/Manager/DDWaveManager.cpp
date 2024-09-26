// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Manager/DDWaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "YSY/AI/AISplineRoute.h"
#include "YSY/Manager/DDEnemySpawnManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Game/DDDataManager.h"
#include "Blueprint/UserWidget.h"
#include "YSY/Game/DDPlayerState.h"

UDDWaveManager::UDDWaveManager()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> VictoryWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/PSA/Widget/04_MainStage/Result/PSA_WBP_Result_Victory.PSA_WBP_Result_Victory_C'"));

	if (VictoryWidgetClassRef.Class)
	{
		VictoryWidgetClass = VictoryWidgetClassRef.Class;
	}

	VictoryWidget = CreateWidget(GetWorld(), VictoryWidgetClass);

	static ConstructorHelpers::FClassFinder<UUserWidget> FailedWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/PSA/Widget/04_MainStage/Result/PSA_WBP_Result_Failed.PSA_WBP_Result_Failed_C'"));

	if (FailedWidgetClassRef.Class)
	{
		FailedWidgetClass = FailedWidgetClassRef.Class;
	}

	FailedWidget = CreateWidget(GetWorld(), FailedWidgetClass);
}

void UDDWaveManager::Initialize()
{
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	auto& TempData = MyGameInstance->GetDataManager()->GetWaveDataTable();

	StageWaveInfo.SetNum(TempData.Num() + 1);

	for (const auto& [Name, Data] : TempData)
	{
		StageWaveInfo[Data.Stage] = Data;
	}

	InitEnemyNames();
}

void UDDWaveManager::InitStage(int32 StageNum)
{
	EnemyOrders.Reset();
	GateOrders.Reset();
	PathGateMappings.Reset();

	CurrentStage = StageNum;
	CurrentWave = 1;
	MaxWave = StageWaveInfo[CurrentStage].EnemyCountsPerWave.Num() - 1;
	RemainingLives = 20;
	EnemyIndex = 0;
	TotalSpawnEnemyCount = 0;
	SetSplines();
	OnSetupEnemyPoolSignature.ExecuteIfBound(StageNum);

	for (auto& Elem : StageWaveInfo[StageNum].EnemyOrder)
	{
		TArray<FString> OutArray{};
		Elem.ParseIntoArray(OutArray, TEXT("#"));

		if (OutArray.Num() >= 2)
		{
			int32 TargetCount = FCString::Atoi(*(OutArray[0]));
			int32 TargetID = FCString::Atoi(*(OutArray[1]));

			while (TargetCount--)
			{
				EnemyOrders.Add(TargetID);
			}
		}

	}

	for (auto& Elem : StageWaveInfo[StageNum].GateOrder)
	{
		TArray<FString> OutArray{};
		Elem.ParseIntoArray(OutArray, TEXT("#"));

		if (OutArray.Num() >= 2)
		{
			int32 TargetCount = FCString::Atoi(*(OutArray[0]));
			int32 TargetID = FCString::Atoi(*(OutArray[1]));

			while (TargetCount--)
			{
				GateOrders.Add(TargetID);
			}

		}
	}

	auto& TempGatePathMappings = StageWaveInfo[StageNum].GatePathMappings;
	int32 PathGateSize = TempGatePathMappings.Num();
	PathGateMappings.SetNum(PathGateSize);

	for (int32 i{}; i < TempGatePathMappings.Num(); i++)
	{
		PathGateMappings[TempGatePathMappings[i]].Add(i);
	}

}

void UDDWaveManager::SetSplines()
{
	Splines.Empty();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISplineRoute::StaticClass(), OutActors);

	for (auto& Actor : OutActors)
	{
		Splines.Add(CastChecked<AAISplineRoute>(Actor));
	}
}

void UDDWaveManager::WaveStart()
{
	if (bIsWaveInProgress || CurrentWave >= StageWaveInfo[CurrentStage].EnemyCountsPerWave.Num())
	{
		return;
	}

	bIsWaveInProgress = true;

	TotalSpawnEnemyCount = 0;

	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, [&]()
		{
			int32 EnemyOrderIndex = EnemyOrders[EnemyIndex];

			int32 RandomSize = PathGateMappings[GateOrders[EnemyIndex]].Num();

			int32 PathOrderIndex = UKismetMathLibrary::RandomInteger(RandomSize);

			OnActivateEnemySignature.Execute(EnemyNames[EnemyOrderIndex], PathGateMappings[GateOrders[EnemyIndex]][PathOrderIndex]);
			EnemyIndex++;

			if (++TotalSpawnEnemyCount >= StageWaveInfo[CurrentStage].EnemyCountsPerWave[CurrentWave])
			{
				SpawnEnd();
			}

		}, 1.0f, true, 0.1f);
}

void UDDWaveManager::SpawnEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("WaveSpawnEnd"));

	GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
}

void UDDWaveManager::WaveEnd()
{
	bIsWaveInProgress = false;
	UE_LOG(LogTemp, Warning, TEXT("WaveEnd"));

	if (CurrentWave++ >= StageWaveInfo[CurrentStage].EnemyCountsPerWave.Num() - 1)
	{
		StageEnd();
		return;
	}

	OnWaveChangedSignature.Broadcast(CurrentWave, MaxWave);

}

void UDDWaveManager::StageEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("StageEnd"));

	int32 GettingLikePoint = RemainingLives / 5;
	if (!GettingLikePoint)
	{
		GettingLikePoint = 1;
	}

	auto TempPlayerState = Cast<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	auto TempSaveGame = TempPlayerState->GetSaveGame();
	if (TempSaveGame->GetStageCleared()[CurrentStage])
	{
		GettingLikePoint -= TempSaveGame->GetStageLikePoints()[CurrentStage];

		if (GettingLikePoint > 0)
		{
			TempPlayerState->AddLikePoint(GettingLikePoint);
		}
	}
	else
	{
		TempPlayerState->AddLikePoint(GettingLikePoint);
	}
	// TODO : YSY Connect GettingLikePoint
	auto TempPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TempPlayerController->StopMovement();
	TempPlayerController->DisableInput(TempPlayerController);
	VictoryWidget->AddToViewport();
	//TempPlayerController->EnableInput(TempPlayerController);
}

void UDDWaveManager::AddEnemyNumber()
{
	TotalEnemyCount++;
}

void UDDWaveManager::SubEnemyNumber()
{
	TotalEnemyCount--;
	if (TotalEnemyCount <= 0)
	{
		WaveEnd();
	}
}

// TODO : YSY Need Change DataInit way
void UDDWaveManager::InitEnemyNames()
{
	EnemyNames.Add(FName("Error"));
	EnemyNames.Add(FName("Ironclad"));
	EnemyNames.Add(FName("Pushfire"));
	EnemyNames.Add(FName("Doomshot"));
	EnemyNames.Add(FName("Legio"));
	EnemyNames.Add(FName("ShockBomber"));
	EnemyNames.Add(FName("Obliterator"));
}

void UDDWaveManager::SubtractRemainingLives(int32 Amount)
{
	RemainingLives -= Amount;

	RemainingLives = std::max(0, RemainingLives);
	if (RemainingLives <= 0)
	{
		FailedWidget->AddToViewport();
	}
	OnRemainingLivesChanged.Broadcast(RemainingLives);
}
