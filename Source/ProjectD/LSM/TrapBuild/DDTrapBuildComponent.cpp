// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapBuild/DDTrapBuildComponent.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDTrapManager.h"
#include "LSM/Trap/DDTrapBase.h"
#include "LSM/Manager/DDBuildManager.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/Collision/CollisionChannel.h"

// Sets default values for this component's properties
UDDTrapBuildComponent::UDDTrapBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDDTrapBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	// TrapManager가 초기화되었는지 확인
	check(GetWorld());
	UDDGameInstance* GameInstance = CastChecked<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(GameInstance);

	TrapManager = GameInstance->GetTrapManager();
	check(TrapManager);

	// BuildManager initialization
	BuildManager = Cast<ADDBuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADDBuildManager::StaticClass()));
	check(BuildManager);

}


// Called every frame
void UDDTrapBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PreviewTrap) {
		UE_LOG(LogTemp, Warning, TEXT("PreviewTrap Success"));
	}

	// ...
}

void UDDTrapBuildComponent::ReadyTrap(const FName& TrapName)
{
	UWorld* World = GetWorld();
	check(World);

	AActor* Owner = GetOwner();
	check(Owner);

	APawn* Instigator = Cast<APawn>(Owner);

	PreviewTrap = TrapManager->SpawnTrap(World, TrapName, FVector(-10000,-10000,-10000), FRotator(), Owner, Instigator);
	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &UDDTrapBuildComponent::PerformTrace, 0.1f, true);

}

void UDDTrapBuildComponent::CancleReadyTrap(ADDTrapBase* Trap)
{
	PreviewTrap = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
}


void UDDTrapBuildComponent::BuildTrap(ADDTrapBase* Trap)
{
	check(TrapManager);

	UWorld* World = GetWorld();
	check(World);

	AActor* Owner = GetOwner();
	check(Owner);

	APawn* Instigator = Cast<APawn>(Owner);

	FVector Location = Owner->GetActorLocation();
	FRotator Rotator = Owner->GetActorRotation();

	//ADDTrapBase* SpawnedTrap = TrapManager->SpawnTrap(World, TrapName, Location, Rotator, Owner, Instigator);

	UE_LOG(LogTemp, Warning, TEXT("TrapReady Success"));
}

void UDDTrapBuildComponent::CancleBuildTrap(ADDTrapBase* Trap)
{
	UE_LOG(LogTemp, Warning, TEXT("CancleTrap Success"));
}

void UDDTrapBuildComponent::UpgradeTrap(ADDTrapBase* Trap)
{
}

void UDDTrapBuildComponent::PerformTrace()
{
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

			if (BuildManager->CanPlaceTrapAtLocation(HitLocation)) {
				FVector NearestCellLocation = BuildManager->GetNearestGridCellLocation(HitLocation);

				PreviewTrap->SetActorLocation(NearestCellLocation);

			}
		}

		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
	}

}

