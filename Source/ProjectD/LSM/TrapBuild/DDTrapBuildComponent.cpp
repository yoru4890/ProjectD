// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapBuild/DDTrapBuildComponent.h"
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

	// ...
	
}


// Called every frame
void UDDTrapBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDDTrapBuildComponent::ReadyTrap(ADDTrapBase* Trap) const
{
	UE_LOG(LogTemp, Warning, TEXT("ReadyTrap Success"));
}

void UDDTrapBuildComponent::CancleReadyTrap(ADDTrapBase* Trap) const
{
}


void UDDTrapBuildComponent::BuildTrap(ADDTrapBase* Trap) const
{
	UE_LOG(LogTemp, Warning, TEXT("TrapBuild Success"));
}

void UDDTrapBuildComponent::CancleBuildTrap(ADDTrapBase* Trap) const
{
	UE_LOG(LogTemp, Warning, TEXT("CancleTrap Success"));
}

void UDDTrapBuildComponent::UpgradeTrap(ADDTrapBase* Trap) const
{
}

