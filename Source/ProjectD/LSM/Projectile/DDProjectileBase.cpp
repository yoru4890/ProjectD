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
		// 활성화: 충돌 활성화, 액터 표시, 틱 활성화
		ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);

		// 나이아가라 이펙트 및 사운드 활성화 (선택 사항)
		if (TrailNiagaraComponent && !TrailNiagaraComponent->IsActive())
		{
			TrailNiagaraComponent->Activate();
		}

		// 필요한 경우 추가 초기화 로직
		CurrentPenetrationCount = 0;  // 관통 횟수 초기화
	}
	else
	{
		// 비활성화: 이동 중지, 충돌 비활성화, 액터 숨김, 틱 비활성화
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->StopMovementImmediately();
			ProjectileMovementComponent->SetActive(false);
		}

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		// 나이아가라 이펙트 및 사운드 비활성화 (선택 사항)
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

	// 폭파이펙트 추가 가능
	if (bIsExplosive)
	{
		// 폭발 처리 로직
	}

	// 프로젝타일 제거
	ProjectileManager->DestroyProjectile(this);
	StopLifeTimeTimer();

}

void ADDProjectileBase::StopLifeTimeTimer()
{
	// 타이머가 설정된 경우 타이머 종료
	if (GetWorld()->GetTimerManager().IsTimerActive(LifeSpanTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimerHandle);
	}
}

#pragma endregion Utility
