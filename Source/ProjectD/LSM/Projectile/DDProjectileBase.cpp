// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Projectile/DDProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "YSY/Collision/CollisionChannel.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Interface/DamageInterface.h"
#include "Engine/DamageEvents.h"
// Sets default values
ADDProjectileBase::ADDProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComoponent"));
	RootComponent = StaticMeshComponent;

	// ���̾ư��� �ý��� ������Ʈ ����
	TrailNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	TrailNiagaraComponent->SetupAttachment(RootComponent);
	TrailNiagaraComponent->SetAutoActivate(false);  // �⺻������ ��Ȱ��ȭ

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("UProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	SetupCollisionResponses();


}

ADDProjectileBase::~ADDProjectileBase()
{
}

// Called when the game starts or when spawned
void ADDProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

#pragma region SetupAndInitialization

void ADDProjectileBase::ConfigureProjectile(float InDamageAmount, TSubclassOf<UDamageType> InDamageType, float InProjectileSpeed, float InMaxSpeed, float InMaxLifeTime, bool InbIsExplosive, float InExplosionRadius, int32 InMaxPenetrationCount)
{
	DamageAmount = InDamageAmount;
	DamageType = InDamageType;
	ProjectileSpeed = InProjectileSpeed;
	MaxSpeed = InMaxSpeed;
	MaxLifeTime = InMaxLifeTime;
	bIsExplosive = InbIsExplosive;
	ExplosionRadius = InExplosionRadius;
	MaxPenetrationCount = InMaxPenetrationCount;

	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &ADDProjectileBase::OnLifeTimeExpired, MaxLifeTime, false);

	LaunchProjectile();
}

void ADDProjectileBase::SetAssetAndManager(const FDDProjectileData& LoadedAsset, UDDProjectileManager* InProjectileManager)
{
	SetMeshs(LoadedAsset);
	SetSound(LoadedAsset);
	SetParticeEffects(LoadedAsset);
	ProjectileManager = InProjectileManager;
}

void ADDProjectileBase::SetupCollisionResponses()
{
	StaticMeshComponent->SetCollisionProfileName("Projectile");
	//StaticMeshComponent->SetCollisionObjectType(GTCHANNEL_PROJECTILE);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADDProjectileBase::OnCollisionBeginOverlap);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ADDProjectileBase::OnCollisionHit);
	StaticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ADDProjectileBase::OnCollisionEndOverlap);
}

void ADDProjectileBase::SetParticeEffects(const FDDProjectileData& LoadedAsset)
{
	if (LoadedAsset.ImpactEffect.IsValid())
	{
		ImpactEffect = LoadedAsset.ImpactEffect.Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : ImpactEffect not loaded"), RowName);
	}

	if (LoadedAsset.TrailEffect.IsValid())
	{
		TrailEffect = LoadedAsset.TrailEffect.Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : ImpactEffect not loaded"), RowName);
	}
}

void ADDProjectileBase::SetSound(const FDDProjectileData& LoadedAsset)
{
	if (LoadedAsset.ImpactSound.IsValid())
	{
		ImpactSound = LoadedAsset.ImpactSound.Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : ImpactSound not loaded"), RowName);
	}

	if (LoadedAsset.FlyingSound.IsValid())
	{
		FlyingSound = LoadedAsset.FlyingSound.Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : FlyingSound not loaded"), RowName);
	}
}

void ADDProjectileBase::SetMeshs(const FDDProjectileData& LoadedAsset)
{
	if (LoadedAsset.FlyingSound.IsValid())
	{
		UStaticMesh* StaticMesh = LoadedAsset.StaticMesh.Get();
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : StaticMesh not loaded"), RowName);
	}
}

#pragma endregion SetupAndInitialization

#pragma region CollisionCallbacks

void ADDProjectileBase::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CurrentPenetrationCount++;

	ApplyDamageToActor(OtherActor);


	if (CurrentPenetrationCount >= MaxPenetrationCount)
	{
		if (bIsExplosive)
		{

		}
		else 
		{

		}
		//ProjectileManager->DestroyProjectile(this);
		//StopLifeTimeTimer();
	}
}

void ADDProjectileBase::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->SetActive(false);
	this->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 50.f);
}

void ADDProjectileBase::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

#pragma endregion CollisionAndCallbacks

#pragma region AttackAndExplode

void ADDProjectileBase::Explode()
{

}

void ADDProjectileBase::ApplyDamageToActor(AActor* OtherActor)
{
	IDamageInterface* DamagedActor = Cast<IDamageInterface>(OtherActor);

	if (DamagedActor)
	{
		FDamageEvent DamageEvent{};
		DamageEvent.DamageTypeClass = DamageType;
		DamagedActor->ApplyDamage(DamageAmount, DamageEvent, nullptr, this);

		UE_LOG(LogTemp, Warning, TEXT("Apply Direct Damage"));
	}
}

#pragma endregion AttackAndExplode

#pragma region TickAndUpdate

// Called every frame
void ADDProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma endregion TickAndUpdate

#pragma region Utility

void ADDProjectileBase::LaunchProjectile()
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->SetActive(true);
		ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
		ProjectileMovementComponent->MaxSpeed = MaxSpeed;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

		ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileSpeed;
	}
}

void ADDProjectileBase::SetProjectileState(bool bIsActive)
{
	if (bIsActive)
	{
		// Ȱ��ȭ: �浹 Ȱ��ȭ, ���� ǥ��, ƽ Ȱ��ȭ
		ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);

		// ���̾ư��� ����Ʈ �� ���� Ȱ��ȭ (���� ����)
		if (TrailNiagaraComponent && !TrailNiagaraComponent->IsActive())
		{
			TrailNiagaraComponent->Activate();
		}

		// �ʿ��� ��� �߰� �ʱ�ȭ ����
		CurrentPenetrationCount = 0;  // ���� Ƚ�� �ʱ�ȭ
	}
	else
	{
		// ��Ȱ��ȭ: �̵� ����, �浹 ��Ȱ��ȭ, ���� ����, ƽ ��Ȱ��ȭ
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->StopMovementImmediately();
			ProjectileMovementComponent->SetActive(false);
		}

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		// ���̾ư��� ����Ʈ �� ���� ��Ȱ��ȭ (���� ����)
		if (TrailNiagaraComponent && TrailNiagaraComponent->IsActive())
		{
			TrailNiagaraComponent->Deactivate();
		}
	}
}

void ADDProjectileBase::SetStaticMeshRotator(FRotator Rotator)
{
	StaticMeshComponent->SetRelativeRotation(Rotator);
}

void ADDProjectileBase::OnLifeTimeExpired()
{
	if (!ProjectileManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileManager is nullptr in OnLifeTimeExpired!"));
		return;
	}

	// ��������Ʈ �߰� ����
	if (bIsExplosive)
	{
		// ���� ó�� ����
	}

	// ������Ÿ�� ����
	ProjectileManager->DestroyProjectile(this);
	StopLifeTimeTimer();

}

void ADDProjectileBase::StopLifeTimeTimer()
{
	// Ÿ�̸Ӱ� ������ ��� Ÿ�̸� ����
	if (GetWorld()->GetTimerManager().IsTimerActive(LifeSpanTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimerHandle);
	}
}

#pragma endregion Utility
