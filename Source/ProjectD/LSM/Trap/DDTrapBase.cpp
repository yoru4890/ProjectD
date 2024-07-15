// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Trap/DDTrapBase.h"

// Sets default values
ADDTrapBase::ADDTrapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//RootComponent = Mesh;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> TrapMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	//if (TrapMeshRef.Object) {
	//	Mesh->SetStaticMesh(TrapMeshRef.Object);
	//}

}

// Called when the game starts or when spawned
void ADDTrapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADDTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADDTrapBase::InitFromDataTable(const FDDTrapStruct& TrapData)
{
	TrapName = TrapData.TrapName;
	TrapType = TrapData.TrapType;
	TrapBuildCost = TrapData.TrapBuildCost;
	TrapUpgradeCost = TrapData.TrapUpgradeCost;
	TrapUnlockCost = TrapData.TrapUnlockCost;
	TrapCoolTime = TrapData.TrapCoolTime;
	TrapDamage = TrapData.TrapDamage;
	TrapLevel = TrapData.TrapLevel;
	TrapParentName = TrapData.TrapParentName;
	TrapChildNames = TrapData.TrapChildNames;
	bIsTrapUnlocked = TrapData.bIsTrapUnlocked;
	TrapMeshType = TrapData.TrapMeshType;
	//TrapStaticMesh = TrapData.TrapStaticMesh.LoadSynchronous();
	//TrapSkeletalMesh = TrapData.TrapSkeletalMesh.LoadSynchronous();
	//TrapAnimBlueprint = TrapData.TrapAnimBlueprint.LoadSynchronous();
	//TrapEffect = TrapData.TrapEffect.LoadSynchronous();
	bIsDotTrap = TrapData.bIsDotTrap;
	DotDamage = TrapData.DotDamage;
	DotDuration = TrapData.DotDuration;
	DotInterval = TrapData.DotInterval;
	bIsSlowTrap = TrapData.bIsSlowTrap;
	SlowAmount = TrapData.SlowAmount;
	SlowDuration = TrapData.SlowDuration;
}

