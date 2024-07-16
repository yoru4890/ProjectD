// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapBuild/DDTrapBuildComponent.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/TrapManager/DDTrapManager.h"
#include "LSM/Trap/DDTrapBase.h"

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

}


// Called every frame
void UDDTrapBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDDTrapBuildComponent::ReadyTrap(const FName& TrapName)
{
	check(TrapManager);

	UWorld* World = GetWorld();
	check(World);

	AActor* Owner = GetOwner();
	check(Owner);

	APawn* Instigator = Cast<APawn>(Owner);

	FVector Location = Owner->GetActorLocation();
	FRotator Rotator = Owner->GetActorRotation();

	ADDTrapBase* SpawnedTrap = TrapManager->SpawnTrap(World, TrapName, Location, Rotator, Owner, Instigator);

	UE_LOG(LogTemp, Warning, TEXT("TrapReady Success"));
}

void UDDTrapBuildComponent::CancleReadyTrap(ADDTrapBase* Trap)
{
}


void UDDTrapBuildComponent::BuildTrap(ADDTrapBase* Trap)
{
	UE_LOG(LogTemp, Warning, TEXT("TrapBuild Success"));
}

void UDDTrapBuildComponent::CancleBuildTrap(ADDTrapBase* Trap)
{
	UE_LOG(LogTemp, Warning, TEXT("CancleTrap Success"));
}

void UDDTrapBuildComponent::UpgradeTrap(ADDTrapBase* Trap)
{
}

