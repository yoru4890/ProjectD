// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Trap/DDTrap.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"

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

void ADDTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADDTrap::Attack()
{
	Super::Attack();

	FDamageEvent DamageEvent{};
	DamageEvent.DamageTypeClass = DamageType;
	for (AActor* Enemy : EnemiesInRanged)
	{
		Enemy->TakeDamage(Damage, DamageEvent, nullptr, this);
		UE_LOG(LogTemp, Warning, TEXT("Trap Attack : %d , Damage Type: %s"), Damage, *DamageType->GetDisplayNameText().ToString());
	}
}

void ADDTrap::ModifyMeshAndAttackCollision() const
{
	Super::ModifyMeshAndAttackCollision();
	FVector AttackRange = FVector(GridCellSize * CellWidth/2, GridCellSize * CellWidth/2, 50);
	UE_LOG(LogTemp, Warning, TEXT("GridCellSize: %d"), GridCellSize);
	UE_LOG(LogTemp, Warning, TEXT("CellWidth: %d"), CellWidth);
	Cast<UBoxComponent>(AttackCollisionComponent)->SetBoxExtent(AttackRange);
}
