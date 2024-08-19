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

// Sets default values for this component's properties
UDDBuildComponent::UDDBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/0000/LSM/Widget/LSM_WB_CantBuild.LSM_WB_CantBuild_C"));
	if (WidgetClassFinder.Succeeded())
	{
		HitWarningWidgetClass = WidgetClassFinder.Class;
	}
}


// Called when the game starts
void UDDBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 클래스를 설정하는 부분은 블루프린트에서 설정 가능
	if (HitWarningWidgetClass)
	{
		HitWarningWidgetInstance = CastChecked<UDDCantBuildWidget>(CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HitWarningWidgetClass));
	}

	// BuildingManager가 초기화되었는지 확인
	check(GetWorld());
	UDDGameInstance* GameInstance = CastChecked<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(GameInstance);

	BuildingManager = GameInstance->GetBuildingManager();
	check(BuildingManager);

	// GridBuildManager initialization
	GridBuildManager = Cast<ADDGridBuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADDGridBuildManager::StaticClass()));
	check(GridBuildManager);

	PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	check(PlayerState);

}


// Called every frame
void UDDBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* UDDBuildComponent::ReadyBuilding(const FName& RowName)
{
	bIsSetBuilding = false;
	if (PreviewBuilding) {
		if (PreviewBuilding->GetRowName() == RowName) {
			return PreviewBuilding;
		}
		else {
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
		return nullptr; // nullptr 반환하여 문제 발생을 방지
	}

	bool bCanPay = CanPayBuildCost(RowName);

	PreviewBuilding->SetMaterialToPreview(bCanPay);

	return PreviewBuilding;

}

void UDDBuildComponent::CancelReadyBuilding()
{
	check(PreviewBuilding);
	BuildingManager->DestroyBuilding(*PreviewBuilding);
	PreviewBuilding = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(BuildTraceTimerHandle);
	if (HitWarningWidgetInstance && HitWarningWidgetInstance->IsInViewport())
	{
		HitWarningWidgetInstance->RemoveFromParent();
	}
}


bool UDDBuildComponent::PlaceBuilding()
{
	check(PreviewBuilding);

	bool bCanPay = CanPayBuildCost(PreviewBuilding->GetRowName());

	if (PreviewBuilding->IsHidden() || !bIsSetBuilding) {
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

	PreviewBuilding = nullptr;

	return true;
}

void UDDBuildComponent::CancelPlacedBuilding()
{
	if (!ManagedBuilding) {
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

bool UDDBuildComponent::UpgradeBuilding(const FName& RowName)
{
	if (!ManagedBuilding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed : ManagedBuilding is null"));
		return false;
	}
	FDDBuildingBaseData& UpgradeBuildingData = *BuildingManager->GetBuildingData(RowName);
	if (!UpgradeBuildingData.bIsUnlocked)
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed : UpgradeBuildingData is Lock"));
		return false;
	}

	if (!CanPayBuildCost(RowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed : Cost Lacked"));
		return false;
	}

	FDDBuildingBaseData& ManagedBuildingData = *BuildingManager->GetBuildingData(ManagedBuilding->GetRowName());

	if (!ManagedBuildingData.ChildRowNames.Contains(RowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrade Failed : Mangaged Building is Final Building"));
		return false;
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
		return false;
	}
	BuildingManager->DestroyBuilding(*ManagedBuilding);
	ManagedBuilding = nullptr;

	ManagedBuilding = NewBuilding;
	ManagedBuilding->SetCanAttack(true);
	ManagedBuilding->SetMaterialToPreview(true);
	PayBuildCost(RowName);
	return true;
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
	}
}

void UDDBuildComponent::StartManageTrace()
{
	GetWorld()->GetTimerManager().SetTimer(ManageTraceTimerHandle, this, &UDDBuildComponent::PerformManageTrace, 0.1f, true);
}

void UDDBuildComponent::PerformBuildTrace()
{
	if (!PreviewBuilding)
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
			check(GridBuildManager);
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
