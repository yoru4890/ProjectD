// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/DDBuildingBase.h"
#include "Components/BoxComponent.h"
#include "LSM/Manager/DDGridBuildManager.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/Collision/CollisionChannel.h"
#include "DDBuildingAnimInstance.h"
#include "NiagaraComponent.h"
#include "LSM/Building/AttackStrategies/DDBuildingBaseAttackStrategy.h"

#pragma region ConstructorAndInitialization
// Sets default values
ADDBuildingBase::ADDBuildingBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a default scene root component
	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/0000/LSM/Building/LSM_MI_PreviewTrap.LSM_MI_PreviewTrap"));
	if (MaterialFinder.Succeeded())
	{
		PreviewMaterial = MaterialFinder.Object;
	}

	MeshContainerComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MashContainerComponent"));
	MeshContainerComponent->SetupAttachment(RootComponent);

	// ���̾ư��� �ý��� ������Ʈ ����
	AttackNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	AttackNiagaraComponent->SetupAttachment(RootComponent);
	AttackNiagaraComponent->SetAutoActivate(false);  // �⺻������ ��Ȱ��ȭ


}

ADDBuildingBase::~ADDBuildingBase()
{
}

// Called when the game starts or when spawned
void ADDBuildingBase::BeginPlay()
{
	Super::BeginPlay();
	if (PreviewMaterial)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(PreviewMaterial, this);
	}

	// ����Ŵ������� Grid ũ�⸦ �����´�.
	ADDGridBuildManager* GridBuildManager = Cast<ADDGridBuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADDGridBuildManager::StaticClass()));
	check(GridBuildManager);

	GridCellSize = GridBuildManager->GetGridCellSize();

}

#pragma endregion ConstructorAndInitialization

#pragma region TickAndUpdate

// Called every frame
void ADDBuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma endregion TickAndUpdate

#pragma region SetupAndInitialization

void ADDBuildingBase::SetupAttackCollisionResponses()
{
	if (AttackCollisionComponent)
	{
		AttackCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		AttackCollisionComponent->SetCollisionResponseToChannel(GTCHANNEL_ENEMY, ECollisionResponse::ECR_Overlap); // Assuming Enemy uses ECC_Pawn
		AttackCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADDBuildingBase::OnBoxCollisionBeginOverlap);
		AttackCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ADDBuildingBase::OnBoxCollisionEndOverlap);
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

void ADDBuildingBase::SetAttackStrategy(TSubclassOf<class UDDBuildingBaseAttackStrategy> AttackStrategyClass)
{
	if (AttackStrategyClass)
	{
		UDDBuildingBaseAttackStrategy* StrategyInstance = NewObject<UDDBuildingBaseAttackStrategy>(this, AttackStrategyClass);

		AttackStrategy = StrategyInstance;
		AttackStrategy->Initialize(this);
	}
}


void ADDBuildingBase::SetAssets(const FDDBuildingBaseData& LoadedAsset)
{
	SetMeshs(LoadedAsset);
	SetParticeEffects(LoadedAsset);
	ModifyMeshAndAttackCollision();
	SetSound(LoadedAsset);
	SetAttackStrategy(LoadedAsset.AttackStrategy);

}

#pragma endregion SetupAndInitialization

#pragma region EffectsAndMeshes

void ADDBuildingBase::SetParticeEffects(const FDDBuildingBaseData& LoadedAsset)
{
	if (LoadedAsset.AttackEffect.IsValid())
	{
		AttackEffect = LoadedAsset.AttackEffect.Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : AttackEffect not loaded"), RowName);
	}

	if (LoadedAsset.HitEffect.IsValid())
	{
		HitEffect = LoadedAsset.HitEffect.Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : HitEffect not loaded"), RowName);
	}

	FName SocketName = TEXT("FirePoint");
	USceneComponent* TargetComponent = nullptr;

	if (SkeletalMeshComponents.Num() > 0 && SkeletalMeshComponents[0]->DoesSocketExist(SocketName))
	{
		TargetComponent = SkeletalMeshComponents[0];
	}
	else if (StaticMeshComponents.Num() > 0 && StaticMeshComponents[0]->DoesSocketExist(SocketName))
	{
		TargetComponent = StaticMeshComponents[0];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket %s not found on any mesh components."), *SocketName.ToString());
		return;
	}

	if (AttackNiagaraComponent)
	{
		AttackNiagaraComponent->AttachToComponent(TargetComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		AttackNiagaraComponent->RegisterComponent();
	}
	if (UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(AttackEffect))
	{
		// NiagaraSystem ���� �� Ȱ��ȭ
		if (AttackNiagaraComponent)
		{
			AttackNiagaraComponent->SetAsset(NiagaraSystem);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackEffect is of unknown type."));
	}
}

void ADDBuildingBase::SetSound(const FDDBuildingBaseData& LoadedAsset)
{
	if (LoadedAsset.AttackSound.IsValid())
	{
		AttackSound = LoadedAsset.AttackSound.Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : AttackSound not loaded"), RowName);
	}
}

void ADDBuildingBase::SetMeshs(const FDDBuildingBaseData& LoadedAsset)
{
	for (UMeshComponent* MeshComponent : MeshComponents)
	{
		if (MeshComponent)
		{
			MeshComponent->DestroyComponent();
		}
	}
	MeshComponents.Empty();

	for (TSoftObjectPtr<UAnimMontage> AnimMontageSoftPtr : LoadedAsset.AttackMontages)
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

	for (TSoftObjectPtr<USkeletalMesh> SkeletalMeshSoftPtr : LoadedAsset.SkeletalMeshs)
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

	for (int index = 0; index < LoadedAsset.AnimBlueprints.Num(); index++)
	{
		if (SkeletalMeshComponents.IsValidIndex(index))
		{
			if (LoadedAsset.AnimBlueprints.IsValidIndex(index) && LoadedAsset.AnimBlueprints[index].IsValid())
			{
				SkeletalMeshComponents[index]->SetAnimInstanceClass(LoadedAsset.AnimBlueprints[index]->GeneratedClass);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s : AnimBlueprints not loaded"), RowName);
			}
		}
	}

	for (TSoftObjectPtr<UStaticMesh> StaticMeshSoftPtr : LoadedAsset.StaticMeshs)
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
		StaticMeshComponents.Add(StaticMeshComponent);
		FDDMaterials MaterialStruct;
		MaterialStruct.Materials = StaticMeshComponent->GetMaterials();
		OriginalMaterials.Add(StaticMeshComponent, MaterialStruct);
	}
}
#pragma endregion EffectsAndMeshes

#pragma region AttackAndCollision

void ADDBuildingBase::SetCanAttack(const bool bInCanAttack)
{
	bCanAttack = bInCanAttack;
	TimeSinceLastAttack = 0;

	if (!bInCanAttack)
	{
		AttackCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StopAttackEffect();
	}
	else
	{
		AttackCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ADDBuildingBase::ExecuteAttackEffects()
{
	PlayAttackEffectAtSocket();
	PlayAttackAnimation();
	PlayAttackSound();
}

void ADDBuildingBase::PlayAttackEffectAtSocket()
{
	AttackNiagaraComponent->SetActive(true);

}

void ADDBuildingBase::StopAttackEffect()
{
	AttackNiagaraComponent->SetActive(false);
}

void ADDBuildingBase::PlayAttackAnimation()
{
	if (!AttackMontages.IsEmpty() && !SkeletalMeshComponents.IsEmpty())
	{
		for (int index = 0; index < SkeletalMeshComponents.Num(); index++)
		{
			UAnimInstance* AnimInstance = SkeletalMeshComponents[index]->GetAnimInstance();
			if (AnimInstance)
			{
				// UDDTrapAnimInstance�� ĳ�����մϴ�.
				UDDBuildingAnimInstance* BuildingAnimInstance = Cast<UDDBuildingAnimInstance>(AnimInstance);
				if (BuildingAnimInstance && AttackMontages.IsValidIndex(index))
				{
					// PlayAnimationMontage�� ȣ���մϴ�.
					BuildingAnimInstance->PlayAnimationMontage(AttackMontages[index]);
				}
			}
		}
	}
}

void ADDBuildingBase::PlayAttackSound()
{
	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
	}
}

#pragma endregion AttackAndCollision

#pragma region CollisionCallbacks

void ADDBuildingBase::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EnemiesInRanged.Add(OtherActor);
}

void ADDBuildingBase::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	EnemiesInRanged.Remove(OtherActor);
}

#pragma endregion CollisionCallbacks

#pragma region UtilityFunctions

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

	FBox CombinedBox(ForceInit); // �� ���� �ʱ�ȭ



	// �� �ڽ� ������Ʈ�� ���� ��踦 ����ϰ� ��ħ
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
			// ��踦 ��ħ
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

	// ������ ���� �� ���� ���� ���� ����Ͽ� �յ��ϰ� ������ ����
	float UniformScaleFactor = FMath::Min(ScaleFactor.X, ScaleFactor.Y);

	// MeshContainerComponent�� ������ ����
	MeshContainerComponent->SetWorldScale3D(FVector(UniformScaleFactor));

	// MeshContainerComponent�� ��ġ ����
	MeshContainerComponent->SetRelativeLocation(FVector(0, 0, MeshZAxisModify));

	UE_LOG(LogTemp, Warning, TEXT("UniformScaleFactor: %f"), UniformScaleFactor);

}


void ADDBuildingBase::SetMaterialToPreview(bool bCanPay)
{
	if (bCanPay)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Param", FLinearColor(0, 0, 0.6f, 1));
	}
	else
	{
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

#pragma endregion UtilityFunctions
