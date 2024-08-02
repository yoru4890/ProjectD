// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/BuildComponent/DDTowerBuildComponent.h"

// Sets default values for this component's properties
UDDTowerBuildComponent::UDDTowerBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDDTowerBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDDTowerBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* UDDTowerBuildComponent::ReadyTower(const FName& RowName)
{
	return nullptr;
}

void UDDTowerBuildComponent::CancleReadyTower()
{
}

bool UDDTowerBuildComponent::BuildTower()
{
	return false;
}

void UDDTowerBuildComponent::CancleBuildTower()
{
}

bool UDDTowerBuildComponent::UpgradeTower(const FName& RowName)
{
	return false;
}

void UDDTowerBuildComponent::AllStopTrace()
{
}

void UDDTowerBuildComponent::StopTowerBuildTrace()
{
}

void UDDTowerBuildComponent::StartTowerBuildTrace()
{
}

void UDDTowerBuildComponent::StopTowerManageTrace()
{
}

void UDDTowerBuildComponent::StartTowerManageTrace()
{
}

void UDDTowerBuildComponent::PerformTowerBuildTrace()
{
}

void UDDTowerBuildComponent::PerformTowerManageTrace()
{
}

bool UDDTowerBuildComponent::CanPayTowerBuildCost(const FName& RowName) const
{
	return false;
}

bool UDDTowerBuildComponent::PayTowerBuildCost(const FName& RowName) const
{
	return false;
}

bool UDDTowerBuildComponent::CanPayTowerUpgradeCost(const FName& RowName) const
{
	return false;
}

bool UDDTowerBuildComponent::PayTowerUpgradeCost(const FName& RowName) const
{
	return false;
}

