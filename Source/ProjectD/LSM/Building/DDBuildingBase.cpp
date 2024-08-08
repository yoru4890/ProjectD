// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/DDBuildingBase.h"
#include "Components/BoxComponent.h"
#include "LSM/Manager/DDGridBuildManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/DamageEvents.h"
#include "YSY/Collision/CollisionChannel.h"

// Sets default values
ADDBuildingBase::ADDBuildingBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a default scene root component
	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	AttackCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionComponent"));
	AttackCollisionComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/0000/LSM/Mesh/LSM_MI_PreviewTrap.LSM_MI_PreviewTrap"));
	if (MaterialFinder.Succeeded())
	{
		PreviewMaterial = MaterialFinder.Object;
	}

	MeshContainerComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MashContainerComponent"));
	MeshContainerComponent->SetupAttachment(RootComponent);


	AttackCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCollisionComponent->SetCollisionResponseToChannel(GTCHANNEL_ENEMY, ECollisionResponse::ECR_Overlap); // Assuming Enemy uses ECC_Pawn
	AttackCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADDBuildingBase::OnBoxCollisionBeginOverlap);
	AttackCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ADDBuildingBase::OnBoxCollisionEndOverlap);

}

ADDBuildingBase::~ADDBuildingBase()
{
}

// Called when the game starts or when spawned
void ADDBuildingBase::BeginPlay()
{
	Super::BeginPlay();
	if (PreviewMaterial) {
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(PreviewMaterial, this);
	}

	// 빌드매니저에서 Grid 크기를 가져온다.
	ADDGridBuildManager* GridBuildManager = Cast<ADDGridBuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADDGridBuildManager::StaticClass()));
	check(GridBuildManager);

	GridCellSize = GridBuildManager->GetGridCellSize();

}

// Called every frame
void ADDBuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSinceLastAttack += DeltaTime;

	if (bCanAttack && !EnemiesInRanged.IsEmpty() && TimeSinceLastAttack >= AttackCoolTime)
	{
		Attack();
		TimeSinceLastAttack = 0.f;
	}
}

void ADDBuildingBase::SetCanAttack(const bool bInCanAttack)
{
	bCanAttack = bInCanAttack;
	TimeSinceLastAttack = 0;

	if (!bInCanAttack)
	{
		AttackCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		AttackCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ADDBuildingBase::InitFromDataTable(const FName& InRowName, const FDDBuildingBaseData& BuildingData)
{
	RowName = InRowName;
	DisplayName = BuildingData.DisplayName;
	BuildingType = BuildingData.BuildingType;
	CellWidth = BuildingData.OccupiedCellWidth;
	AttackCoolTime = BuildingData.AttackCoolTime;
	Damage = BuildingData.Damage;
	bIsAnimated = BuildingData.bIsAnimated;
	bIsDot = BuildingData.bIsDot;
	DotDamage = BuildingData.DotDamage;
	DotDuration = BuildingData.DotDuration;
	DotInterval = BuildingData.DotInterval;
	bIsSlow = BuildingData.bIsSlow;
	SlowAmount = BuildingData.SlowAmount;
	SlowDuration = BuildingData.SlowDuration;
	bCanAttack = false;
	DamageType = BuildingData.DamageType;
	MeshZAxisModify = BuildingData.MeshZAxisModify;
	UE_LOG(LogTemp, Warning, TEXT("MeshZAxisModify is : %f"), BuildingData.MeshZAxisModify);
}

void ADDBuildingBase::SetAssets(FDDBuildingBaseData& LoadedAsset)
{
	SetParticeEffects(LoadedAsset);
	SetMeshs(LoadedAsset);
	ModifyMeshAndAttackCollision();

}

void ADDBuildingBase::SetParticeEffects(FDDBuildingBaseData& LoadedAsset)
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
		if (ParticlEffectSoftPtr.IsValid())
		{
			ParticleEffectComponent->SetTemplate(ParticlEffectSoftPtr.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : ParticleEffect not loaded"), RowName);
		}
		ParticleEffectComponent->RegisterComponent();

		ParticleEffectComponents.Add(ParticleEffectComponent);
	}
}

void ADDBuildingBase::SetMeshs(FDDBuildingBaseData& LoadedAsset)
{
	for (UMeshComponent* MeshComponent : MeshComponents)
	{
		if (MeshComponent)
		{
			MeshComponent->DestroyComponent();
		}
	}
	MeshComponents.Empty();

	for (TSoftObjectPtr<UAnimMontage>& AnimMontageSoftPtr : LoadedAsset.AttackMontages)
	{
		if (AnimMontageSoftPtr.IsValid())
		{
			AttackMontages.Add(AnimMontageSoftPtr.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : Montages not loaded"), RowName);
		}
	}

	for (TSoftObjectPtr<USkeletalMesh>& SkeletalMeshSoftPtr : LoadedAsset.SkeletalMeshs) 
	{
		USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
		check(SkeletalMeshComponent);

		SkeletalMeshComponent->SetupAttachment(MeshContainerComponent);
		if (SkeletalMeshSoftPtr.IsValid())
		{
			SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshSoftPtr.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : SkeletalMesh not loaded"), RowName);
		}
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SkeletalMeshComponent->SetCollisionResponseToChannel(GTCHANNEL_MANAGETRACE, ECR_Block);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SkeletalMeshComponent->RegisterComponent();

		MeshComponents.Add(SkeletalMeshComponent);
		SkeletalMeshComponents.Add(SkeletalMeshComponent);
		FDDMaterials MaterialStruct;
		MaterialStruct.Materials = SkeletalMeshComponent->GetMaterials();
		OriginalMaterials.Add(SkeletalMeshComponent, MaterialStruct);
	}

	if (LoadedAsset.MyAnimBlueprint.IsValid())
	{
		for (auto& SkeletalMeshComponent : SkeletalMeshComponents) 
		{
			SkeletalMeshComponent->SetAnimInstanceClass(LoadedAsset.MyAnimBlueprint->GeneratedClass);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : AnimBluePrint not loaded"), RowName);
	}

	for (TSoftObjectPtr<UStaticMesh>& StaticMeshSoftPtr : LoadedAsset.StaticMeshs)
	{
		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		check(StaticMeshComponent);

		StaticMeshComponent->SetupAttachment(MeshContainerComponent);
		if (StaticMeshSoftPtr.IsValid())
		{
			StaticMeshComponent->SetStaticMesh(StaticMeshSoftPtr.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : StaticMesh not loaded"), RowName);
		}
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(GTCHANNEL_MANAGETRACE, ECR_Block);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		StaticMeshComponent->RegisterComponent();

		MeshComponents.Add(StaticMeshComponent);
		FDDMaterials MaterialStruct;
		MaterialStruct.Materials = StaticMeshComponent->GetMaterials();
		OriginalMaterials.Add(StaticMeshComponent, MaterialStruct);
	}
}

void ADDBuildingBase::ModifyMeshAndAttackCollision() const
{
	if (!MeshContainerComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshContainerComponent is not valid."));
		return;
	}

	if (MeshComponents.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshComponents array is empty."));
		return;
	}

	FBox CombinedBox(ForceInit); // 빈 경계로 초기화



	// 각 자식 컴포넌트에 대해 경계를 계산하고 합침
	for (UMeshComponent* MeshComponent : MeshComponents)
	{
		if (!MeshComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("MeshComponent is not valid."));
			continue;
		}

		FBox ComponentBox(ForceInit);

		if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(MeshComponent))
		{
			if (UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh())
			{
				ComponentBox = StaticMesh->GetBoundingBox();
			}
		}
		else if (USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(MeshComponent))
		{
			ComponentBox = SkeletalMeshComponent->Bounds.GetBox();
		}

		if (ComponentBox.IsValid)
		{
			// 경계를 합침
			CombinedBox += ComponentBox.TransformBy(MeshComponent->GetComponentTransform());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ComponentBox is not valid for %s."), *MeshComponent->GetName());
		}
	}

	FVector CurrentSize = CombinedBox.GetSize();
	FVector TargetSize(CellWidth * GridCellSize, CellWidth * GridCellSize, CellWidth * GridCellSize);
	FVector ScaleFactor = TargetSize / CurrentSize;

	UE_LOG(LogTemp, Warning, TEXT("CurrentSize X:%f Y:%f Z:%f"), CurrentSize.X, CurrentSize.Y, CurrentSize.Z);
	UE_LOG(LogTemp, Warning, TEXT("TargetSize X:%f Y:%f Z:%f"), TargetSize.X, TargetSize.Y, TargetSize.Z);
	UE_LOG(LogTemp, Warning, TEXT("ScaleFactor X:%f Y:%f Z:%f"), ScaleFactor.X, ScaleFactor.Y, ScaleFactor.Z);

	// 스케일 팩터 중 가장 작은 값을 사용하여 균등하게 스케일 조정
	float UniformScaleFactor = FMath::Min(ScaleFactor.X, ScaleFactor.Y);

	// MeshContainerComponent에 스케일 적용
	MeshContainerComponent->SetWorldScale3D(FVector(UniformScaleFactor));

	// MeshContainerComponent의 위치 설정
	MeshContainerComponent->SetRelativeLocation(FVector(0, 0, MeshZAxisModify));

	UE_LOG(LogTemp, Warning, TEXT("UniformScaleFactor: %f"), UniformScaleFactor);

}


void ADDBuildingBase::SetMaterialToPreview(bool bCanPay)
{
	if (bCanPay) {
		DynamicMaterialInstance->SetVectorParameterValue("Param", FLinearColor(0, 0, 0.6f, 1));
	}
	else {
		DynamicMaterialInstance->SetVectorParameterValue("Param", FLinearColor(0.6, 0, 0, 1));
	}

	for (auto& OriginalMaterial : OriginalMaterials)
	{
		for (int32 MaterialIndex = 0; MaterialIndex < OriginalMaterial.Value.Materials.Num(); ++MaterialIndex)
		{
			OriginalMaterial.Key->SetMaterial(MaterialIndex, DynamicMaterialInstance);
		}
	}
}

void ADDBuildingBase::SetMaterialToOriginal()
{
	for (auto& OriginalMaterial : OriginalMaterials)
	{
		for (int32 MaterialIndex = 0; MaterialIndex < OriginalMaterial.Value.Materials.Num(); ++MaterialIndex)
		{
			OriginalMaterial.Key->SetMaterial(MaterialIndex, OriginalMaterial.Value.Materials[MaterialIndex]);
		}
	}
}

void ADDBuildingBase::Attack()
{
	FDamageEvent DamageEvent{};
	DamageEvent.DamageTypeClass = DamageType;
	for (AActor* Enemy : EnemiesInRanged) {
		Enemy->TakeDamage(Damage, DamageEvent, nullptr, this);
		UE_LOG(LogTemp, Warning, TEXT("Trap Attack : %d , Damage Type: %s"), Damage, *DamageType->GetDisplayNameText().ToString());
	}
}

void ADDBuildingBase::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EnemiesInRanged.Add(OtherActor);
}

void ADDBuildingBase::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	EnemiesInRanged.Remove(OtherActor);
}
