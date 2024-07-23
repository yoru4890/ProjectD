// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Trap/DDTrapBase.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADDTrapBase::ADDTrapBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize the BoxComponent
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));
	RootComponent = BoxCollisionComponent;
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
	TrapName = TrapData.Name;
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
	TrapMeshType = TrapData.MeshType;
	bIsDotTrap = TrapData.bIsDotTrap;
	DotDamage = TrapData.DotDamage;
	DotDuration = TrapData.DotDuration;
	DotInterval = TrapData.DotInterval;
	bIsSlowTrap = TrapData.bIsSlowTrap;
	SlowAmount = TrapData.SlowAmount;
	SlowDuration = TrapData.SlowDuration;
}

void ADDTrapBase::SetTrapAssets(TArray<UStaticMesh*> StaticMeshs, TArray<USkeletalMesh*> SkeletalMeshs, UAnimBlueprint* AnimBlueprint, TArray<UParticleSystem*> ParticleEffects)
{
	// 기존 ParticleEffectComponents 배열 초기화
	for (UParticleSystemComponent* ParticleEffectComponent : ParticleEffectComponents)
	{
		if (ParticleEffectComponent)
		{
			ParticleEffectComponent->DestroyComponent();
		}
	}
	ParticleEffectComponents.Empty();

	for (auto* ParticlEffect : ParticleEffects) {
		UParticleSystemComponent* ParticleEffectComponent = NewObject<UParticleSystemComponent>(this);
		check(ParticleEffectComponent);
		// ParticleRootComponent에 부착
		ParticleEffectComponent->SetupAttachment(RootComponent);
		ParticleEffectComponent->SetTemplate(ParticlEffect);
		ParticleEffectComponent->RegisterComponent();

		ParticleEffectComponents.Add(ParticleEffectComponent);
	}
}

void ADDTrapBase::SetAttachParticleToRoot()
{
	//for (UParticleSystemComponent* ParticleEffectComponent : ParticleEffectComponents) {
	//	check(ParticleEffectComponent);
	//	ParticleEffectComponent->SetupAttachment(RootComponent);
	//	ParticleEffectComponent->RegisterComponent();
	//	ParticleEffectComponent->SetRelativeScale3D(FVector(1.f));
	//}
}
