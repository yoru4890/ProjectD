// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapBuild/DDTrapBuildComponent.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDTrapManager.h"
#include "LSM/Trap/DDTrapBase.h"
#include "LSM/Manager/DDBuildManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "YSY/Collision/CollisionChannel.h"
#include "YSY/Game/DDPlayerState.h"
#include "LSM/Widget/DDCantBuildWidget.h"

// Sets default values for this component's properties
UDDTrapBuildComponent::UDDTrapBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/0000/LSM/Widget/LSM_WB_CantBuild.LSM_WB_CantBuild_C"));
	if (WidgetClassFinder.Succeeded())
	{
		HitWarningWidgetClass = WidgetClassFinder.Class;
	}
}


// Called when the game starts
void UDDTrapBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 클래스를 설정하는 부분은 블루프린트에서 설정 가능
	if (HitWarningWidgetClass)
	{
		HitWarningWidgetInstance = CastChecked<UDDCantBuildWidget>(CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HitWarningWidgetClass));
	}

	// TrapManager가 초기화되었는지 확인
	check(GetWorld());
	UDDGameInstance* GameInstance = CastChecked<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(GameInstance);

	TrapManager = GameInstance->GetTrapManager();
	check(TrapManager);

	// BuildManager initialization
	BuildManager = Cast<ADDBuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADDBuildManager::StaticClass()));
	check(BuildManager);

	PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	check(PlayerState);

}


// Called every frame
void UDDTrapBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PreviewTrap) {

	}

	// ...
}

AActor* UDDTrapBuildComponent::ReadyTrap(const FName& RowName)
{
	bIsSetTrap = false;
	if (PreviewTrap) {
		if (PreviewTrap->GetTrapRowName() == RowName) {
			return PreviewTrap;
		}
		else {
			CancleReadyTrap();
		}
	}
	UWorld* World = GetWorld();
	check(World);

	AActor* Owner = GetOwner();
	check(Owner);

	APawn* Instigator = Cast<APawn>(Owner);

	PreviewTrap = TrapManager->SpawnTrap(World, RowName, FVector(-10000, -10000, -10000), FRotator(0, 0, 0), Owner, Instigator);
	/*GetWorld()->GetTimerManager().SetTimer(TrapBuildTraceTimerHandle, this, &UDDTrapBuildComponent::PerformTrapBuildTrace, 0.1f, true);*/
	bool bCanPay = CanPayTrapBuildCost(RowName);
	PreviewTrap->SetMaterialToPreview(bCanPay);

	return PreviewTrap;

}

void UDDTrapBuildComponent::CancleReadyTrap()
{
	check(PreviewTrap);
	TrapManager->DestroyTrap(*PreviewTrap);
	PreviewTrap = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(TrapBuildTraceTimerHandle);
	if (HitWarningWidgetInstance && HitWarningWidgetInstance->IsInViewport())
	{
		HitWarningWidgetInstance->RemoveFromParent();
	}
}


bool UDDTrapBuildComponent::BuildTrap()
{
	check(PreviewTrap);

	bool bCanPay = CanPayTrapBuildCost(PreviewTrap->GetTrapRowName());

	if (PreviewTrap->IsHidden() || !bIsSetTrap) {
		return false;
	}

	if (!bCanPay) {
		return false;
	}

	PreviewTrap->SetMaterialToOriginal();
	/*GetWorld()->GetTimerManager().ClearTimer(TrapBuildTraceTimerHandle);*/
	BuildManager->SetGridCellAsOccupied(PreviewTrap->GetActorLocation());

	const FName& TrapRowName = PreviewTrap->GetTrapRowName();

	PreviewTrap->SetTrapCanAttack(true);
	PayTrapBuildCost(TrapRowName);

	PreviewTrap = nullptr;
	ReadyTrap(TrapRowName);

	//UE_LOG(LogTemp, Warning, TEXT("TrapBuild Success"));
	//UE_LOG(LogTemp, Warning, TEXT("Player Gold : %d"), PlayerState->GetGold());

	return true;
}

void UDDTrapBuildComponent::CancleBuildTrap()
{
	if (!ManagedTrap) {
		return;
	}
	FDDTrapStruct& ManagedTrapData = TrapManager->GetTrapData(ManagedTrap->GetTrapRowName());
	BuildManager->SetGridCellAsBlank(ManagedTrap->GetActorLocation());
	PlayerState->AddGold(ManagedTrapData.TrapBuildCost * 0.8f);
	TrapManager->DestroyTrap(*ManagedTrap);
	ManagedTrap->SetTrapCanAttack(false);
	ManagedTrap = nullptr;
}

bool UDDTrapBuildComponent::UpgradeTrap(const FName& RowName)
{
	if (!ManagedTrap) 
	{
		return false;
	}
	FDDTrapStruct& UpgradeTrapData = TrapManager->GetTrapData(RowName);
	if (!UpgradeTrapData.bIsTrapUnlocked)
	{
		return false;
	}

	if (!CanPayTrapUpgradeCost(RowName)) 
	{
		return false;
	}

	FDDTrapStruct& ManagedTrapData = TrapManager->GetTrapData(ManagedTrap->GetTrapRowName());

	if (!ManagedTrapData.TrapChildRowNames.Contains(RowName))
	{
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Upgrade Gold : %d"), UpgradeTrapData.TrapUpgradeCost);

	UWorld* World = GetWorld();
	check(World);

	AActor* Owner = GetOwner();
	check(Owner);

	APawn* Instigator = Cast<APawn>(Owner);

	ADDTrapBase* NewTrap = TrapManager->SpawnTrap(World, RowName, ManagedTrap->GetActorLocation(), FRotator(0, 0, 0), Owner, Instigator);
	TrapManager->DestroyTrap(*ManagedTrap);

	ManagedTrap = NewTrap;
	ManagedTrap->SetTrapCanAttack(true);
	ManagedTrap->SetMaterialToPreview(true);
	PayTrapUpgradeCost(RowName);
	return true;
}

void UDDTrapBuildComponent::AllStopTrace()
{
	StopTrapBuildTrace();
	StopTrapManageTrace();
}

void UDDTrapBuildComponent::StopTrapBuildTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TrapBuildTraceTimerHandle);
	if (PreviewTrap)
	{
		TrapManager->DestroyTrap(*PreviewTrap);
		PreviewTrap = nullptr;
	}

	if (HitWarningWidgetInstance && HitWarningWidgetInstance->IsInViewport())
	{
		HitWarningWidgetInstance->RemoveFromParent();
	}
}

void UDDTrapBuildComponent::StartTrapBuildTrace()
{
	GetWorld()->GetTimerManager().SetTimer(TrapBuildTraceTimerHandle, this, &UDDTrapBuildComponent::PerformTrapBuildTrace, 0.1f, true);
}

void UDDTrapBuildComponent::StopTrapManageTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TrapManageTraceTimerHandle);
	if (ManagedTrap)
	{
		ManagedTrap->SetMaterialToOriginal();
		ManagedTrap = nullptr;
	}
}

void UDDTrapBuildComponent::StartTrapManageTrace()
{
	GetWorld()->GetTimerManager().SetTimer(TrapManageTraceTimerHandle, this, &UDDTrapBuildComponent::PerformTrapManageTrace, 0.1f, true);
}

void UDDTrapBuildComponent::PerformTrapBuildTrace()
{
	//UE_LOG(LogTemp, Warning, TEXT("PerformTrapBuildTrace Success"));
	//UE_LOG(LogTemp, Warning, TEXT("Player Gold : %d"),PlayerState->GetGold());
	if (!PreviewTrap)
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
			FVector HitLocation = HitResult.Location;
			check(BuildManager);
			check(HitWarningWidgetInstance);

			if (BuildManager->CanPlaceTrapAtLocation(HitLocation)) {
				FVector NearestCellLocation = BuildManager->GetNearestGridCellLocation(HitLocation);

				check(PreviewTrap);
				PreviewTrap->SetActorLocation(NearestCellLocation);
				PreviewTrap->SetActorHiddenInGame(false);

				// Hide the warning widget if there is a hit
				if (HitWarningWidgetInstance && HitWarningWidgetInstance->IsInViewport())
				{
					if (CanPayTrapBuildCost(PreviewTrap->GetTrapRowName()))
					{
						HitWarningWidgetInstance->RemoveFromParent();
					}
					else
					{
						HitWarningWidgetInstance->ShowCantPayImage();
					}
				}

			}
			else
			{
				PreviewTrap->SetActorHiddenInGame(true);
				if (HitWarningWidgetInstance && !HitWarningWidgetInstance->IsInViewport())
				{
					HitWarningWidgetInstance->AddToViewport();
				}

				HitWarningWidgetInstance->ShowCantPlaceImage();
			}
		}
		else
		{
			PreviewTrap->SetActorHiddenInGame(true);
			if (HitWarningWidgetInstance && !HitWarningWidgetInstance->IsInViewport())
			{
				HitWarningWidgetInstance->AddToViewport();
			}
			HitWarningWidgetInstance->ShowCantPlaceImage();
		}
	}
	bIsSetTrap = true;

}

void UDDTrapBuildComponent::PerformTrapManageTrace()
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

			if (ManagedTrap != HitActor)
			{
				if (ManagedTrap)
				{
					ManagedTrap->SetMaterialToOriginal();
				}
				ManagedTrap = Cast<ADDTrapBase>(HitActor);// HitResult에서 맞은 Actor를 가져옴
				ManagedTrap->SetMaterialToPreview(true);
				//UE_LOG(LogTemp, Warning, TEXT("Manage Trap Changed"));
			}
		}
		else {
			if (ManagedTrap) {
				ManagedTrap->SetMaterialToOriginal();
				ManagedTrap = nullptr;
			}
		}
	}
}

bool UDDTrapBuildComponent::CanPayTrapBuildCost(const FName& RowName) const
{
	const FDDTrapStruct& TrapData = TrapManager->GetTrapData(RowName);
	bool bCanPay = PlayerState->CheckGold(TrapData.TrapBuildCost);
	return bCanPay;
}

bool UDDTrapBuildComponent::PayTrapBuildCost(const FName& RowName) const
{
	const FDDTrapStruct& TrapData = TrapManager->GetTrapData(RowName);
	bool bIsPay = PlayerState->SubtractGold(TrapData.TrapBuildCost);
	return bIsPay;
}

bool UDDTrapBuildComponent::CanPayTrapUpgradeCost(const FName& RowName) const
{
	const FDDTrapStruct& TrapData = TrapManager->GetTrapData(RowName);
	bool bCanPay = PlayerState->CheckGold(TrapData.TrapUpgradeCost);
	return bCanPay;
}

bool UDDTrapBuildComponent::PayTrapUpgradeCost(const FName& RowName) const
{
	const FDDTrapStruct& TrapData = TrapManager->GetTrapData(RowName);
	bool bIsPay = PlayerState->SubtractGold(TrapData.TrapUpgradeCost);
	return bIsPay;
}
