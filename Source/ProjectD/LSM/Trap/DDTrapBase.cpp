// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Trap/DDTrapBase.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "YSY/Collision/CollisionChannel.h"

// Sets default values
ADDTrapBase::ADDTrapBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize the BoxComponent
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->SetBoxExtent(FVector(150.0f, 150.0f, 40.0f));
	BoxCollisionComponent->SetCollisionResponseToChannel(GTCHANNEL_MANAGETRACE, ECR_Block);
	RootComponent = BoxCollisionComponent;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/0000/LSM/Mesh/Trap/LSM_MI_PreviewTrap.LSM_MI_PreviewTrap"));
	if (MaterialFinder.Succeeded())
	{
		PreviewMaterial = MaterialFinder.Object;
	}
}

ADDTrapBase::~ADDTrapBase()
{

}

// Called when the game starts or when spawned
void ADDTrapBase::BeginPlay()
{
	Super::BeginPlay();
	if (PreviewMaterial) {
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(PreviewMaterial, this);
	}

}

// Called every frame
void ADDTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADDTrapBase::InitFromDataTable(const FName& RowName, const FDDTrapStruct& TrapData)
{
	TrapRowName = RowName;
	TrapDisplayName = TrapData.DisplayName;
	//TrapBuildCost = TrapData.TrapBuildCost;
	//TrapUpgradeCost = TrapData.TrapUpgradeCost;
	//TrapUnlockCost = TrapData.TrapUnlockCost;
	TrapCoolTime = TrapData.TrapCoolTime;
	TrapDamage = TrapData.TrapDamage;
	//TrapLevel = TrapData.TrapLevel;
	//TrapParentName = TrapData.TrapParentRowName;
	//TrapChildNames = TrapData.TrapChildRowNames;
	//bIsTrapUnlocked = TrapData.bIsTrapUnlocked;
	TrapMeshType = TrapData.MeshType;
	bIsDotTrap = TrapData.bIsDotTrap;
	DotDamage = TrapData.DotDamage;
	DotDuration = TrapData.DotDuration;
	DotInterval = TrapData.DotInterval;
	bIsSlowTrap = TrapData.bIsSlowTrap;
	SlowAmount = TrapData.SlowAmount;
	SlowDuration = TrapData.SlowDuration;
	bCanAttack = false;
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

void ADDTrapBase::SetMaterialToPreview(bool bCanPay)
{
	if (bCanPay) {
		DynamicMaterialInstance->SetVectorParameterValue("Param", FLinearColor(0, 0, 0.6f, 1));
	}else{
		DynamicMaterialInstance->SetVectorParameterValue("Param", FLinearColor(0.6, 0, 0, 1));
	}
	if (TrapMeshType == EMeshType::StaticMesh) {
		TArray<UStaticMeshComponent*> Components;
		GetComponents<UStaticMeshComponent>(Components);

		for (int a = 0; a < Components.Num(); a++) {
			for (int b = 0; b < OriginalMaterials[a].Materials.Num(); b++) {
				Components[a]->SetMaterial(b, DynamicMaterialInstance);
			}
		}
	}
	else if (TrapMeshType == EMeshType::SkeletalMesh) {
		TArray<USkeletalMeshComponent*> Components;
		GetComponents<USkeletalMeshComponent>(Components);

		for (int a = 0; a < Components.Num(); a++) {
			for (int b = 0; b < OriginalMaterials[a].Materials.Num(); b++) {
				Components[a]->SetMaterial(b, DynamicMaterialInstance);
			}
		}
	}
}

void ADDTrapBase::SetMaterialToOriginal()
{
	if (TrapMeshType == EMeshType::StaticMesh) {
		TArray<UStaticMeshComponent*> Components;
		GetComponents<UStaticMeshComponent>(Components);

		for (int a = 0; a < Components.Num(); a++) {
			for (int b = 0; b < OriginalMaterials[a].Materials.Num(); b++) {
				Components[a]->SetMaterial(b, OriginalMaterials[a].Materials[b]);
			}
		}
	}
	else if (TrapMeshType == EMeshType::SkeletalMesh) {
		TArray<USkeletalMeshComponent*> Components;
		GetComponents<USkeletalMeshComponent>(Components);

		for (int a = 0; a < Components.Num(); a++) {
			for (int b = 0; b < OriginalMaterials[a].Materials.Num(); b++) {
				Components[a]->SetMaterial(b, OriginalMaterials[a].Materials[b]);
			}
		}
	}
}

