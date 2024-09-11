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
// 
// TODO: 이펙트가 존재하지 않는 경우는 플래그를 풀에서 꺼낼떄마다 True로 바꿔야함
ADDProjectileBase::ADDProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComoponent"));
	RootComponent = StaticMeshComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("UProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	SetupCollisionResponses();

	// 초기화 상태 플래그
	bTrailEffectFinished = true;
	bImpactEffectFinished = true;
	bExplosionEffectFinished = true;


}

ADDProjectileBase::~ADDProjectileBase()
{
}

// Called when the game starts or when spawned
void ADDProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADDProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetActorLocation().Z < HeightToPool)
	{
		StopLifeTimeTimer();
		ProjectileManager->DestroyProjectile(this);
	}
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

	LaunchProjectile();
}

void ADDProjectileBase::SetAssetAndManager(const FDDProjectileData& LoadedAsset, UDDProjectileManager* InProjectileManager)
{
	SetMeshs(LoadedAsset);
	SetSound(LoadedAsset);
	SetParticeEffects(LoadedAsset);
	SetAttachNiagaraComponent();
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

	if (LoadedAsset.ExplosionEffect.IsValid())
	{
		ExplosionEffect = LoadedAsset.ExplosionEffect.Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : ExplosionEffect not loaded"), RowName);
	}
}

void ADDProjectileBase::SetAttachNiagaraComponent()
{
	USceneComponent* TargetComponent = nullptr;
	FName TrailSocketName = FName("TrailPoint");
	if (StaticMeshComponent->DoesSocketExist(TrailSocketName))
	{
		TargetComponent = StaticMeshComponent;
	}
	if (TargetComponent && TrailEffect)
	{
		UNiagaraComponent* NewNiagaraComponent = NewObject<UNiagaraComponent>(this);
		TrailNiagaraComponent = NewNiagaraComponent;
		TrailNiagaraComponent->SetAsset(TrailEffect);
		TrailNiagaraComponent->SetupAttachment(TargetComponent, TrailSocketName);
		TrailNiagaraComponent->SetAutoActivate(false);  // 기본적으로 비활성화
		TrailNiagaraComponent->RegisterComponent();

		TrailNiagaraComponent->OnSystemFinished.AddDynamic(this, &ADDProjectileBase::OnTrailEffectFinished);
	}

	if (ImpactEffect)
	{
		UNiagaraComponent* NewNiagaraComponent = NewObject<UNiagaraComponent>(this);
		ImpactNiagaraComponent = NewNiagaraComponent;
		ImpactNiagaraComponent->SetAsset(ImpactEffect);
		ImpactNiagaraComponent->SetAutoActivate(false);  // 기본적으로 비활성화
		ImpactNiagaraComponent->RegisterComponent();

		ImpactNiagaraComponent->OnSystemFinished.AddDynamic(this, &ADDProjectileBase::OnImpactEffectFinished);
	}

	if (ExplosionEffect)
	{
		UNiagaraComponent* NewNiagaraComponent = NewObject<UNiagaraComponent>(this);
		ExplosionNiagaraComponent = NewNiagaraComponent;
		ExplosionNiagaraComponent->SetAsset(ExplosionEffect);
		ExplosionNiagaraComponent->SetupAttachment(RootComponent);
		ExplosionNiagaraComponent->SetAutoActivate(false);  // 기본적으로 비활성화
		ExplosionNiagaraComponent->RegisterComponent();

		ExplosionNiagaraComponent->OnSystemFinished.AddDynamic(this, &ADDProjectileBase::OnExplosionEffectFinished);
	}
}

void ADDProjectileBase::HandleEffectCompletion()
{
	if (bTrailEffectFinished && bImpactEffectFinished && bExplosionEffectFinished)
	{
		// 나이아가라 이펙트가 끝나면 미사일을 풀로 보냄
		if (ProjectileManager)
		{
			/*UE_LOG(LogTemp, Warning, TEXT("OnTrailEffectFinished Called"));*/
			StopLifeTimeTimer();  // 타이머 정지
			ProjectileManager->DestroyProjectile(this);  // 풀에 돌려보냄
		}
	}
}

void ADDProjectileBase::AllStopNiagaraEffect()
{
	if (TrailNiagaraComponent)
	{
		TrailNiagaraComponent->Deactivate();
	}
	if (ExplosionNiagaraComponent)
	{
		ExplosionNiagaraComponent->Deactivate();
	}
	if (ImpactNiagaraComponent)
	{
		ImpactNiagaraComponent->Deactivate();
	}
}

void ADDProjectileBase::OnTrailEffectFinished(UNiagaraComponent* PSystem)
{
	bTrailEffectFinished = true;
	HandleEffectCompletion();
}

void ADDProjectileBase::OnImpactEffectFinished(UNiagaraComponent* PSystem)
{
	bImpactEffectFinished = true;
	HandleEffectCompletion();
}

void ADDProjectileBase::OnExplosionEffectFinished(UNiagaraComponent* PSystem)
{
	bExplosionEffectFinished = true;
	HandleEffectCompletion();
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
	if (LoadedAsset.StaticMesh.IsValid())
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

	UE_LOG(LogTemp, Warning, TEXT("Projectile Begin ovelap"));

	if (CurrentPenetrationCount >= MaxPenetrationCount)
	{
		if (bIsExplosive)
		{
			if (ExplosionNiagaraComponent)
			{
				ExplosionNiagaraComponent->SetActive(true);
			}
		}
		else 
		{
			ApplyDamageToActor(OtherActor);
		}

		if (!TrailNiagaraComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnCollisionBeginOverlap Called"));
			StopLifeTimeTimer();
			ProjectileManager->DestroyProjectile(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TrailNiagaraComponent->Deactivate()"));
			ProjectileMovementComponent->SetActive(false);
			StaticMeshComponent->SetVisibility(false);
			TrailNiagaraComponent->Deactivate();
		}
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

		GetWorld()->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &ADDProjectileBase::OnLifeTimeExpired, MaxLifeTime, false);
	}
}

void ADDProjectileBase::SetProjectileState(bool bIsActive)
{
	if (bIsActive)
	{
		// 활성화: 충돌 활성화, 액터 표시, 틱 활성화
		ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
		StaticMeshComponent->SetVisibility(true);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);

		// 나이아가라 이펙트 및 사운드 활성화 (선택 사항)
		TrailNiagaraComponent->SetActive(true);
		bTrailEffectFinished = false;

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

		bTrailEffectFinished = true;
		bImpactEffectFinished = true;
		bExplosionEffectFinished = true;

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		// 나이아가라 이펙트 및 사운드 비활성화 (선택 사항)
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
	UE_LOG(LogTemp, Error, TEXT("Projectile is Destroyed because of LifeTime"));
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
