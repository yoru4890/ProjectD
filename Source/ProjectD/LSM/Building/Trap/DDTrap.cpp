// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Trap/DDTrap.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "LSM/Building/AttackStrategies/DDBuildingAttackStrategyInterface.h"

ADDTrap::ADDTrap()
{
	AttackCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionComponent"));
	AttackCollisionComponent->SetupAttachment(RootComponent);
	SetupAttackCollisionResponses();
}

void ADDTrap::BeginPlay()
{
	Super::BeginPlay();
}

void ADDTrap::Attack()
{
	if (!EnemiesInRanged.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		TSet<TObjectPtr<AActor>> Targets = EnemiesInRanged;
		for (auto& Target : Targets)
		{
			AttackStrategy->Attack(Target,nullptr);
		}
	}
}

void ADDTrap::ModifyMeshAndAttackCollision() const
{
	Super::ModifyMeshAndAttackCollision();
	FVector AttackRange = FVector(GridCellSize * CellWidth/2, GridCellSize * CellWidth/2, 50);
	//UE_LOG(LogTemp, Warning, TEXT("GridCellSize: %d"), GridCellSize);
	//UE_LOG(LogTemp, Warning, TEXT("CellWidth: %d"), CellWidth);
	Cast<UBoxComponent>(AttackCollisionComponent)->SetBoxExtent(AttackRange);
}

void ADDTrap::ResetCanAttack()
{
	Super::ResetCanAttack();

	if (EnemiesInRanged.Num() > 0)
	{
		ExecuteAttackEffects();
	}
}

void ADDTrap::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxCollisionBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (bCanAttack)
	{
		ExecuteAttackEffects();
	}
}
