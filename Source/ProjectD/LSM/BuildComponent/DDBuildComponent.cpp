// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/BuildComponent/DDBuildComponent.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDBuildingManager.h"
#include "LSM/Manager/DDGridBuildManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "YSY/Collision/CollisionChannel.h"
#include "YSY/Game/DDPlayerState.h"
#include "LSM/Widget/DDCantBuildWidget.h"
#include "LSM/Building/DDBuildingBase.h"
#include "LSM/Widget/DDSelectBuildingWidget.h"
#include "LSM/Widget/DDStartBuildWidget.h"
#include "LSM/Widget/DDUpgradeBuildingWidget.h"

// Sets default values for this component's properties
UDDBuildComponent::UDDBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> HitWarningWidgetRef(TEXT("/Game/0000/LSM/Widget/LSM_WB_CantBuild.LSM_WB_CantBuild_C"));
	if (HitWarningWidgetRef.Succeeded())
	{
		HitWarningWidgetClass = HitWarningWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UDDSelectBuildingWidget> SelectBuildingWidgetRef(TEXT("/Game/0000/LSM/Widget/BuildComponent/LSM_WBP_RM_SelectBuilding.LSM_WBP_RM_SelectBuilding_C"));

	if (SelectBuildingWidgetRef.Succeeded())
	{
		SelectBuildingWidgetClass = SelectBuildingWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UDDStartBuildWidget> StartBuildWidgetRef(TEXT("/Game/0000/LSM/Widget/BuildComponent/LSM_WBP_RM_StartBuild.LSM_WBP_RM_StartBuild_C"));

	if (StartBuildWidgetRef.Succeeded())
	{
		StartBuildWidgetClass = StartBuildWidgetRef.Class;
	}


	static ConstructorHelpers::FClassFinder<UDDUpgradeBuildingWidget> UpgradeBuildingWidgetOPtion1Ref(TEXT("/Game/0000/LSM/Widget/BuildComponent/LSM_WBP_RM_UpgradeBuilding_Option1.LSM_WBP_RM_UpgradeBuilding_Option1_C"));

	if (UpgradeBuildingWidgetOPtion1Ref.Succeeded())
	{
		UpgradeBuildingWidgetOption1Class = UpgradeBuildingWidgetOPtion1Ref.Class;
	}


	static ConstructorHelpers::FClassFinder<UDDUpgradeBuildingWidget> UpgradeBuildingWidgetOPtion2Ref(TEXT("/Game/0000/LSM/Widget/BuildComponent/LSM_WBP_RM_UpgradeBuilding_Option2.LSM_WBP_RM_UpgradeBuilding_Option2_C"));

	if (UpgradeBuildingWidgetOPtion2Ref.Succeeded())
	{
		UpgradeBuildingWidgetOption2Class = UpgradeBuildingWidgetOPtion2Ref.Class;
	}

}


// Called when the game starts
void UDDBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	InitWidget();
	BindEventsToWidget();
	// BuildingManager가 초기화되었는지 확인
	check(GetWorld());
	UDDGameInstance* GameInstance = CastChecked<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(GameInstance);

	BuildingManager = GameInstance->GetBuildingManager();
	check(BuildingManager);

	// GridBuildManager initialization
	GridBuildManager = Cast<ADDGridBuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADDGridBuildManager::StaticClass()));

	PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	check(PlayerState);

}

void UDDBuildComponent::InitWidget()
{
	// 위젯 클래스를 설정하는 부분은 블루프린트에서 설정 가능
	if (HitWarningWidgetClass)
	{
		HitWarningWidgetInstance = CastChecked<UDDCantBuildWidget>(CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HitWarningWidgetClass));
	}

	if (SelectBuildingWidgetClass)
	{
		SelectBuildingWidgetInstance = CreateWidget<UDDSelectBuildingWidget>(GetWorld(), SelectBuildingWidgetClass);
	}

	if (StartBuildWidgetClass)
	{
		StartBuildWidgetInstance = CreateWidget<UDDStartBuildWidget>(GetWorld(), StartBuildWidgetClass);
	}

	if (UpgradeBuildingWidgetOption1Class)
	{
		UpgradeBuildingWidgetOption1Instance = CreateWidget<UDDUpgradeBuildingWidget>(GetWorld(), UpgradeBuildingWidgetOption1Class);
	}

	if (UpgradeBuildingWidgetOption2Class)
	{
		UpgradeBuildingWidgetOption2Instance = CreateWidget<UDDUpgradeBuildingWidget>(GetWorld(), UpgradeBuildingWidgetOption2Class);
	}
}

void UDDBuildComponent::BindEventsToWidget()
{
	if (StartBuildWidgetInstance)
	{
		StartBuildWidgetInstance->OnBuildingTypeSelected.AddDynamic(this, &UDDBuildComponent::ShowSelectBuildingWidget);
	}

	if (SelectBuildingWidgetInstance)
	{
		SelectBuildingWidgetInstance->OnBuildingSelected.AddDynamic(this, &UDDBuildComponent::ReadyBuilding);
	}

	if (SelectBuildingWidgetInstance)
	{
		SelectBuildingWidgetInstance->OnBuildingSelectionCanceled.AddDynamic(this, &UDDBuildComponent::ShowStartBuildWidget);
	}

	if (UpgradeBuildingWidgetOption1Instance)
	{
		UpgradeBuildingWidgetOption1Instance->OnSellBuildingSelcted.AddDynamic(this, &UDDBuildComponent::CancelPlacedBuilding);
	}

	if (UpgradeBuildingWidgetOption1Instance)
	{
		UpgradeBuildingWidgetOption1Instance->OnUpgradeBuildingSelected.AddDynamic(this, &UDDBuildComponent::UpgradeBuilding);
	}

	if (UpgradeBuildingWidgetOption2Instance)
	{
		UpgradeBuildingWidgetOption2Instance->OnSellBuildingSelcted.AddDynamic(this, &UDDBuildComponent::CancelPlacedBuilding);
	}
}

void UDDBuildComponent::ShowSelectBuildingWidget(EBuildingType BuildingType)
{
	if (SelectBuildingWidgetInstance)
	{
		SelectBuildingWidgetInstance->SetBuildingType(BuildingType);
		SelectBuildingWidgetInstance->AddToViewport();
	}
}

void UDDBuildComponent::ShowStartBuildWidget()
{
	if (StartBuildWidgetInstance)
	{
		if (StartBuildWidgetInstance->IsInViewport() || SelectBuildingWidgetInstance->IsInViewport())
		{
			return;
		}
		StartBuildWidgetInstance->AddToViewport();
	}
}

void UDDBuildComponent::ShowUpgradeBuildingWidget()
{
	if(!ManagedBuilding)
	{
		return;
	}
	if (UpgradeBuildingWidgetOption1Instance && UpgradeBuildingWidgetOption2Instance)
	{
		if (UpgradeBuildingWidgetOption1Instance->IsInViewport() || UpgradeBuildingWidgetOption2Instance->IsInViewport())
		{
			return;
		}
		const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(ManagedBuilding->GetRowName());
		TArray<FName> ChildrenRownames = BuildingData.ChildRowNames;

		if (ChildrenRownames.Num() >= 2)
		{
			UpgradeBuildingWidgetOption2Instance->SetBuildingRowName(ManagedBuilding->GetRowName());
			UpgradeBuildingWidgetOption2Instance->SetBuildingType(ManagedBuilding->GetBuildingType());
			UpgradeBuildingWidgetOption2Instance->AddToViewport();
		}
		else
		{
			UpgradeBuildingWidgetOption1Instance->SetBuildingRowName(ManagedBuilding->GetRowName());
			UpgradeBuildingWidgetOption1Instance->SetBuildingType(ManagedBuilding->GetBuildingType());
			UpgradeBuildingWidgetOption1Instance->AddToViewport();
		}

	}
}

void UDDBuildComponent::ReadyBuilding(FName RowName)
{
	AllStopTrace();
	if (RowName==NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name None?"));
		return;
	}
	if (!GridBuildManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridBuildManager is not"));
		return;
	}
	bIsSetBuilding = false;
	if (PreviewBuilding) {
		if (PreviewBuilding->GetRowName() == RowName) {
			UE_LOG(LogTemp, Warning, TEXT("Already preview building"));
			StartBuildTrace();
			OnStartBuild.Broadcast();
			return;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("New Preview Building"));
			CancelReadyBuilding();
		}
	}
	UWorld* World = GetWorld();
	check(World);

	AActor* Owner = GetOwner();
	check(Owner);

	APawn* Instigator = Cast<APawn>(Owner);

	PreviewBuilding = BuildingManager->SpawnBuilding(World, RowName, FVector(-10000, -10000, -10000), FRotator(0, 0, 0), Owner, Instigator);

	// 추가: PreviewBuilding이 nullptr인지 확인
	if (!PreviewBuilding) {
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn trap: PreviewBuilding is nullptr"));
		return; // nullptr 반환하여 문제 발생을 방지
	}

	bool bCanPay = CanPayBuildCost(RowName);

	PreviewBuilding->SetMaterialToPreview(bCanPay);

	if (PreviewBuilding->GetBuildingType() == EBuildingType::Tower)
	{
		SetTowerZoneIsHiddenInGame(false);
	}
	StartBuildTrace();
	OnStartBuild.Broadcast();
	return;

}

void UDDBuildComponent::CancelReadyBuilding()
{
	check(PreviewBuilding);
	BuildingManager->DestroyBuilding(*PreviewBuilding);
	PreviewBuilding = nullptr;
	SetTowerZoneIsHiddenInGame(true);
	AllStopTrace();
	StartManageTrace();
}


bool UDDBuildComponent::PlaceBuilding()
{
	check(PreviewBuilding);

	bool bCanPay = CanPayBuildCost(PreviewBuilding->GetRowName());

	if (PreviewBuilding->IsHidden() || !bIsSetBuilding || !GridBuildManager) {
		return false;
	}

	if (!bCanPay) {
		return false;
	}

	PreviewBuilding->SetMaterialToOriginal();

	GridBuildManager->SetGridCellAsOccupied(PreviewBuilding->GetActorLocation(), PreviewBuilding->GetCellWidth());

	const FName& BuildingRowName = PreviewBuilding->GetRowName();

	PreviewBuilding->SetCanAttack(true);
	PayBuildCost(BuildingRowName);

	if (PreviewBuilding->GetBuildingType() == EBuildingType::Tower)
	{
		SetTowerZoneIsHiddenInGame(true);
	}
	PreviewBuilding = nullptr;

	ReadyBuilding(BuildingRowName);

	return true;
}

void UDDBuildComponent::CancelPlacedBuilding()
{
	if (!ManagedBuilding || !GridBuildManager) {
		return;
	}
	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(ManagedBuilding->GetRowName());

	int32 SellCost = BuildingData.SellCost;

	UE_LOG(LogTemp, Warning, TEXT("SellCost is %d"), SellCost);
	GridBuildManager->SetGridCellAsBlank(ManagedBuilding->GetActorLocation(), ManagedBuilding->GetCellWidth());
	PlayerState->AddGold(SellCost);
	BuildingManager->DestroyBuilding(*ManagedBuilding);
	ManagedBuilding = nullptr;
}

void UDDBuildComponent::UpgradeBuilding(const FName RowName)
{
	if (!ManagedBuilding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed : ManagedBuilding is null"));
		return;
	}
	FDDBuildingBaseData& UpgradeBuildingData = *BuildingManager->GetBuildingData(RowName);
	if (!UpgradeBuildingData.bIsUnlocked)
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed : UpgradeBuildingData is Lock"));
		return;
	}

	if (!CanPayBuildCost(RowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed : Cost Lacked"));
		return;
	}

	FDDBuildingBaseData& ManagedBuildingData = *BuildingManager->GetBuildingData(ManagedBuilding->GetRowName());

	if (!ManagedBuildingData.ChildRowNames.Contains(RowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed : Mangaged Building is Final Building"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Upgrade Gold : %d"), UpgradeBuildingData.BuildCost);

	UWorld* World = GetWorld();
	check(World);

	AActor* Owner = GetOwner();
	check(Owner);

	APawn* Instigator = Cast<APawn>(Owner);


	ADDBuildingBase* NewBuilding = BuildingManager->SpawnBuilding(World, RowName, ManagedBuilding->GetActorLocation(), ManagedBuilding->GetActorRotation(), Owner, Instigator);
	if (!NewBuilding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed"));
		return;
	}
	BuildingManager->DestroyBuilding(*ManagedBuilding);
	ManagedBuilding = nullptr;

	ManagedBuilding = NewBuilding;
	ManagedBuilding->SetCanAttack(true);
	ManagedBuilding->SetMaterialToPreview(true);
	PayBuildCost(RowName);
	return;
}

FName UDDBuildComponent::GetManagedBuildingRowName()
{
	if (!ManagedBuilding)
	{
		return NAME_None;
	}
	return ManagedBuilding->GetRowName();
}

void UDDBuildComponent::AllStopTrace()
{
	StopBuildTrace();
	StopManageTrace();
}

void UDDBuildComponent::StopBuildTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(BuildTraceTimerHandle);
	if (PreviewBuilding)
	{
		BuildingManager->DestroyBuilding(*PreviewBuilding);
		PreviewBuilding = nullptr;
	}

	if (HitWarningWidgetInstance && HitWarningWidgetInstance->IsInViewport())
	{
		HitWarningWidgetInstance->RemoveFromParent();
	}
	if (StartBuildWidgetInstance)
	{
		StartBuildWidgetInstance->RemoveFromParent();
	}
	if (SelectBuildingWidgetInstance)
	{
		SelectBuildingWidgetInstance->RemoveFromParent();
	}
}

void UDDBuildComponent::StartBuildTrace()
{
	GetWorld()->GetTimerManager().SetTimer(BuildTraceTimerHandle, this, &UDDBuildComponent::PerformBuildTrace, 0.1f, true);
}

void UDDBuildComponent::StopManageTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(ManageTraceTimerHandle);
	if (ManagedBuilding)
	{
		ManagedBuilding->SetMaterialToOriginal();
		ManagedBuilding = nullptr;
		if (UpgradeBuildingWidgetOption1Instance->IsInViewport())
		{
			UpgradeBuildingWidgetOption1Instance->RemoveFromParent();
		}

		if (UpgradeBuildingWidgetOption2Instance->IsInViewport())
		{
			UpgradeBuildingWidgetOption2Instance->RemoveFromParent();
		}
	}
}

void UDDBuildComponent::StartManageTrace()
{
	GetWorld()->GetTimerManager().SetTimer(ManageTraceTimerHandle, this, &UDDBuildComponent::PerformManageTrace, 0.1f, true);
}

void UDDBuildComponent::PerformBuildTrace()
{
	if (!PreviewBuilding||!GridBuildManager)
	{
		return;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector Start = CameraLocation;
		FVector End = Start + (CameraRotation.Vector() * 2000.f); // Adjust the range as needed

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, GTCHANNEL_BUILDINGTRACE, CollisionParams);

		if (bHit)
		{
			FVector BuildLocation = HitResult.Location;
			FRotator TowerRotation;
			bool CanBuildAtLoction = false;
			check(HitWarningWidgetInstance);

			if (PreviewBuilding->GetBuildingType() == EBuildingType::Trap)
			{
				CanBuildAtLoction = GridBuildManager->CanPlaceBuildingAtLocation(BuildLocation, PreviewBuilding->GetCellWidth(), false);
			}
			else if (PreviewBuilding->GetBuildingType() == EBuildingType::Tower)
			{
				UPrimitiveComponent* HitComponent = HitResult.GetComponent();
				if (HitComponent)
				{
					BuildLocation = HitComponent->GetComponentLocation();
					CanBuildAtLoction = GridBuildManager->CanPlaceBuildingAtLocation(BuildLocation, PreviewBuilding->GetCellWidth(), true);
					TowerRotation = HitComponent->GetComponentRotation();
				}
				else
				{
					CanBuildAtLoction = false;
				}
			}



			if (CanBuildAtLoction) {
				FVector NearestCellLocation = GridBuildManager->GetNearestGridCellLocation(BuildLocation);
				PreviewBuilding->SetActorLocation(NearestCellLocation);
				FVector NormalVector = GridBuildManager->GetGridCellNormalVector(BuildLocation);
				FRotator ActorRotation = FRotationMatrix::MakeFromZ(NormalVector).Rotator();
				if (PreviewBuilding->GetBuildingType() == EBuildingType::Tower)
				{
					ActorRotation = TowerRotation;
				}
				PreviewBuilding->SetActorRotation(ActorRotation);
				PreviewBuilding->SetActorHiddenInGame(false);

				if (CanPayBuildCost(PreviewBuilding->GetRowName()))
				{
					if (HitWarningWidgetInstance && HitWarningWidgetInstance->IsInViewport())
					{
						HitWarningWidgetInstance->RemoveFromParent();
					}
				}
				else
				{
					if (HitWarningWidgetInstance)
					{
						if (!HitWarningWidgetInstance->IsInViewport())
						{
							HitWarningWidgetInstance->AddToViewport();
						}
						HitWarningWidgetInstance->ShowCantPayImage();
					}
				}
			}
			else
			{
				PreviewBuilding->SetActorHiddenInGame(true);
				if (HitWarningWidgetInstance && !HitWarningWidgetInstance->IsInViewport())
				{
					HitWarningWidgetInstance->AddToViewport();
				}

				HitWarningWidgetInstance->ShowCantPlaceImage();
			}
		}
		else
		{
			PreviewBuilding->SetActorHiddenInGame(true);
			if (HitWarningWidgetInstance && !HitWarningWidgetInstance->IsInViewport())
			{
				HitWarningWidgetInstance->AddToViewport();
			}
			HitWarningWidgetInstance->ShowCantPlaceImage();
		}
	}
	bIsSetBuilding = true;

}

void UDDBuildComponent::PerformManageTrace()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector Start = CameraLocation;
		FVector End = Start + (CameraRotation.Vector() * 1000.f); // Adjust the range as needed

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, GTCHANNEL_MANAGETRACE, CollisionParams);

		if (bHit)
		{
			FVector HitLocation = HitResult.Location;
			AActor* HitActor = HitResult.GetActor();

			if (ManagedBuilding != HitActor)
			{
				if (ManagedBuilding)
				{
					ManagedBuilding->SetMaterialToOriginal();
				}
				ManagedBuilding = Cast<ADDBuildingBase>(HitActor);// HitResult에서 맞은 Actor를 가져옴
				ManagedBuilding->SetMaterialToPreview(true);
				//UE_LOG(LogTemp, Warning, TEXT("Manage Building Changed"));
			}
		}
		else {
			if (ManagedBuilding) {
				ManagedBuilding->SetMaterialToOriginal();
				ManagedBuilding = nullptr;
			}
		}
	}
}

bool UDDBuildComponent::CanPayBuildCost(const FName& RowName) const
{
	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(RowName); 
	bool bCanPay = PlayerState->CheckGold(BuildingData.BuildCost);
	return bCanPay;
}

bool UDDBuildComponent::PayBuildCost(const FName& RowName) const
{
	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(RowName);
	bool bIsPay = PlayerState->SubtractGold(BuildingData.BuildCost);
	return bIsPay;
}

void UDDBuildComponent::SetTowerZoneIsHiddenInGame(bool bIsHiddenInGame) const
{
	if (!GridBuildManager)
	{
		return;
	}
	GridBuildManager->SetTowerBuildingZoneMaterial(bIsHiddenInGame);
}

//bool UDDBuildComponent::CanPayUpgradeCost(const FName& RowName) const
//{
//	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(RowName);
//	bool bCanPay = PlayerState->CheckGold(BuildingData.UpgradeCost);
//	return bCanPay;
//}
//
//bool UDDBuildComponent::PayUpgradeCost(const FName& RowName) const
//{
//	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(RowName);
//	bool bIsPay = PlayerState->SubtractGold(BuildingData.UpgradeCost);
//	return bIsPay;
//}
