// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Projectile/DDProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "YSY/Collision/CollisionChannel.h"
// Sets default values
ADDProjectileBase::ADDProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComoponent"));
	RootComponent = StaticMeshComponent;

	// 나이아가라 시스템 컴포넌트 생성
	TrailNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	TrailNiagaraComponent->SetupAttachment(RootComponent);
	TrailNiagaraComponent->SetAutoActivate(false);  // 기본적으로 비활성화

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

void ADDProjectileBase::InitializeProjectile(float InDamageAmount, TSubclassOf<UDamageType> InDamageType, float InProjectileSpeed, float InMaxLifeTime, bool InbIsExplosive, float InExplosionRadius, int32 InMaxPenetrationCount)
{
	DamageAmount = InDamageAmount;
	DamageType = InDamageType;
	ProjectileSpeed = InProjectileSpeed;
	MaxLifeTime = InMaxLifeTime;
	bIsExplosive = InbIsExplosive;
	ExplosionRadius = InExplosionRadius;
	MaxPenetrationCount = InMaxPenetrationCount;

}

void ADDProjectileBase::SetAssets(const FDDProjectileData& LoadedAsset)
{
	SetMeshs(LoadedAsset);
	SetSound(LoadedAsset);
	SetParticeEffects(LoadedAsset);
}

void ADDProjectileBase::SetupCollisionResponses()
{
	StaticMeshComponent->SetCollisionObjectType(GTCHANNEL_PROJECTILE);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADDProjectileBase::OnCollisionBeginOverlap);
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

	if (CurrentPenetrationCount >= MaxPenetrationCount)
	{
		if (bIsExplosive)
		{

		}
		else
		{

		}
	}
}

void ADDProjectileBase::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ADDProjectileBase::SetProjectileActive(bool bIsActive)
{
	if (!bIsActive)
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	CurrentPenetrationCount = 0;
}

#pragma endregion CollisionAndCallbacks

#pragma region TickAndUpdate

// Called every frame
void ADDProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma endregion TickAndUpdate
