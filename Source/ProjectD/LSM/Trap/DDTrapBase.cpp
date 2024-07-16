// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Trap/DDTrapBase.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADDTrapBase::ADDTrapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ParticleEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	ParticleEffectComponent->SetupAttachment(RootComponent);

}

ADDTrapBase::~ADDTrapBase()
{

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
	bIsDotTrap = TrapData.bIsDotTrap;
	DotDamage = TrapData.DotDamage;
	DotDuration = TrapData.DotDuration;
	DotInterval = TrapData.DotInterval;
	bIsSlowTrap = TrapData.bIsSlowTrap;
	SlowAmount = TrapData.SlowAmount;
	SlowDuration = TrapData.SlowDuration;
}

void ADDTrapBase::SetTrapAssets(UStaticMesh* StaticMesh, USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBlueprint, UParticleSystem* ParticleEffect)
{
	if (ParticleEffect && ParticleEffectComponent) {
		ParticleEffectComponent->SetTemplate(ParticleEffect);
	}
}
