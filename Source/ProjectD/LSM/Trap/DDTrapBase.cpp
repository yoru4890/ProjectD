// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Trap/DDTrapBase.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "YSY/Collision/CollisionChannel.h"
#include "Engine/DamageEvents.h"
#include "LSM/Manager/DDBuildManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADDTrapBase::ADDTrapBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize the BoxComponent
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->SetBoxExtent(FVector(150.0f, 150.0f, 40.0f));
	BoxCollisionComponent->SetCollisionProfileName(TEXT("Trap"), true);
	RootComponent = BoxCollisionComponent;
	BoxCollisionComponent->SetCollisionObjectType(GTCHANNEL_TRAP); // Assuming ECC_GameTraceChannel1 is the new Trap channel
	BoxCollisionComponent->SetCollisionResponseToChannel(GTCHANNEL_ENEMY, ECollisionResponse::ECR_Overlap); // Assuming Enemy uses ECC_Pawn

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/0000/LSM/Mesh/LSM_MI_PreviewTrap.LSM_MI_PreviewTrap"));
	if (MaterialFinder.Succeeded())
	{
		PreviewMaterial = MaterialFinder.Object;
	}

	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADDTrapBase::OnBoxCollisionBeginOverlap);
	BoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ADDTrapBase::OnBoxCollisionEndOverlap);

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

	// 빌드매니저에서 Grid 크기를 가져온다.
	ADDBuildManager* BuildManager = Cast<ADDBuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADDBuildManager::StaticClass()));
	check(BuildManager);

	GridCellSize = BuildManager->GetGridCellSize();

}

// Called every frame
void ADDTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastAttack += DeltaTime;

	if (bCanAttack && !TrappedEnemies.IsEmpty() && TimeSinceLastAttack >= TrapCoolTime)
	{
		Attack();
		TimeSinceLastAttack = 0.f;
	}

}
void ADDTrapBase::SetTrapCanAttack(const bool bInCanAttack)
{
	bCanAttack = bInCanAttack;
	TimeSinceLastAttack = 0;

	if (!bInCanAttack) 
	{
		BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ADDTrapBase::InitFromDataTable(const FName& RowName, const FDDTrapData& TrapData)
{
	TrapRowName = RowName;
	TrapDisplayName = TrapData.DisplayName;
	TrapCellWidth = TrapData.OccupiedCellWidth;
	TrapCoolTime = TrapData.TrapCoolTime;
	TrapDamage = TrapData.TrapDamage;
	TrapMeshType = TrapData.MeshType;
	bIsDotTrap = TrapData.bIsDotTrap;
	DotDamage = TrapData.DotDamage;
	DotDuration = TrapData.DotDuration;
	DotInterval = TrapData.DotInterval;
	bIsSlowTrap = TrapData.bIsSlowTrap;
	SlowAmount = TrapData.SlowAmount;
	SlowDuration = TrapData.SlowDuration;
	bCanAttack = false;
	TrapDamageType = TrapData.DamageType;
	TrapMeshZAxisModify = TrapData.MeshZAxisModify;
	UE_LOG(LogTemp, Warning, TEXT("TrapMeshZAxisModify is : %f"), TrapData.MeshZAxisModify);
}

void ADDTrapBase::SetTrapAssets(FDDBuildingBaseData& LoadedAsset)
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

	for (TSoftObjectPtr<UParticleSystem>& ParticlEffectSoftPtr : LoadedAsset.Effects) {
		UParticleSystemComponent* ParticleEffectComponent = NewObject<UParticleSystemComponent>(this);
		check(ParticleEffectComponent);
		// ParticleRootComponent에 부착
		ParticleEffectComponent->SetupAttachment(RootComponent);
		if(ParticlEffectSoftPtr.IsValid())
		{
			ParticleEffectComponent->SetTemplate(ParticlEffectSoftPtr.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : ParticleEffect not loaded"), TrapRowName);
		}
		ParticleEffectComponent->RegisterComponent();

		ParticleEffectComponents.Add(ParticleEffectComponent);
	}
}

void ADDTrapBase::SetMaterialToPreview(bool bCanPay)
{
	if (bCanPay) {
		DynamicMaterialInstance->SetVectorParameterValue("Param", FLinearColor(0, 0, 0.6f, 1));
	}
	else {
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

void ADDTrapBase::Attack()
{
	FDamageEvent DamageEvent{};
	DamageEvent.DamageTypeClass = TrapDamageType;
	for (AActor* Enemy : TrappedEnemies) {
		Enemy->TakeDamage(TrapDamage, DamageEvent, nullptr, this);
		UE_LOG(LogTemp, Warning, TEXT("Trap Attack : %d , Damage Type: %s"), TrapDamage, *TrapDamageType->GetDisplayNameText().ToString());
	}
}

void ADDTrapBase::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TrappedEnemies.Add(OtherActor);
}

void ADDTrapBase::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TrappedEnemies.Remove(OtherActor);
}
