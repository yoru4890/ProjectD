// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Projectile/DDProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "YSY/Collision/CollisionChannel.h"
#include "LSM/Manager/DDProjectileManager.h"
#include "YSY/Interface/DamageInterface.h"
#include "Engine/DamageEvents.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
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
	bIsInPool = true;
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

void ADDProjectileBase::BeginDestroy()
{
	StopLifeTimeTimer();

	Super::BeginDestroy();

	// 액터가 파괴될 때 타이머를 안전하게 제거
}

void ADDProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 액터가 게임 플레이에서 종료될 때 타이머 제거
	StopLifeTimeTimer();
	Super::EndPlay(EndPlayReason);

}

void ADDProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetActorLocation().Z < HeightToPool)
	{
		ProjectileMovementComponent->SetActive(false);
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
	if (!InProjectileManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileManager is null in SetAssetAndManager!"));
		return;
	}

	SetMeshs(LoadedAsset);
	SetSound(LoadedAsset);
	SetParticeEffects(LoadedAsset);
	//SetAttachAudioComponent();

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
		UNiagaraSystem* ImpactEffectAsset = LoadedAsset.ImpactEffect.Get();
		if (ImpactEffectAsset)
		{
			ImpactEffect = ImpactEffectAsset;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s : Failed to load ImpactEffect from LoadedAsset"), *RowName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : ImpactEffect is not valid in LoadedAsset"), *RowName.ToString());
	}

	if (LoadedAsset.TrailEffect.IsValid())
	{
		UNiagaraSystem* TrailEffectAsset = LoadedAsset.TrailEffect.Get();
		if (TrailEffectAsset)
		{
			TrailEffect = TrailEffectAsset;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s : Failed to load TrailEffect from LoadedAsset"), *RowName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : TrailEffect is not valid in LoadedAsset"), *RowName.ToString());
	}

	if (LoadedAsset.ExplosionEffect.IsValid())
	{
		UNiagaraSystem* ExplosionEffectAsset = LoadedAsset.ExplosionEffect.Get();
		if (ExplosionEffectAsset)
		{
			ExplosionEffect = ExplosionEffectAsset;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s : Failed to load ExplosionEffect from LoadedAsset"), *RowName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : ExplosionEffect is not valid in LoadedAsset"), *RowName.ToString());
	}
}

void ADDProjectileBase::SetSound(const FDDProjectileData& LoadedAsset)
{
	if (LoadedAsset.ImpactSound.IsValid())
	{
		USoundBase* ImpactSoundAsset = LoadedAsset.ImpactSound.Get();
		if (ImpactSoundAsset)
		{
			ImpactSound = ImpactSoundAsset;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s : Failed to load ImpactSound from LoadedAsset"), *RowName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : ImpactSound is not valid in LoadedAsset"), *RowName.ToString());
	}

	if (LoadedAsset.FlyingSound.IsValid())
	{
		USoundBase* FlyingSoundAsset = LoadedAsset.FlyingSound.Get();
		if (FlyingSoundAsset)
		{
			FlyingSound = FlyingSoundAsset;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s : Failed to load FlyingSound from LoadedAsset"), *RowName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : FlyingSound is not valid in LoadedAsset"), *RowName.ToString());
	}


}

void ADDProjectileBase::SetAttachAudioComponent()
{
	if (FlyingSound)
	{
		UAudioComponent* NewAudioComponent = NewObject<UAudioComponent>(this);
		FlyingAudioComponent = NewAudioComponent;
		FlyingAudioComponent->SetupAttachment(RootComponent);
		FlyingAudioComponent->SetAutoActivate(false);
		FlyingAudioComponent->SetSound(FlyingSound);
		FlyingAudioComponent->RegisterComponent();
	}
}

void ADDProjectileBase::SetMeshs(const FDDProjectileData& LoadedAsset)
{
    if (LoadedAsset.StaticMesh.IsValid())
    {
        UStaticMesh* StaticMesh = LoadedAsset.StaticMesh.Get();
        if (StaticMesh)
        {
            StaticMeshComponent->SetStaticMesh(StaticMesh);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load StaticMesh from LoadedAsset"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("StaticMesh is not valid in LoadedAsset"));
    }
}

#pragma endregion SetupAndInitialization

#pragma region CollisionCallbacks

void ADDProjectileBase::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CurrentPenetrationCount++;

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, OtherActor->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("Impact Sound Play"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Projectile Begin ovelap"));
}

void ADDProjectileBase::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//ProjectileMovementComponent->StopMovementImmediately();
	//ProjectileMovementComponent->SetActive(false);
	//this->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 50.f);
}

void ADDProjectileBase::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

#pragma endregion CollisionAndCallbacks

#pragma region AttackAndExplode

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

		if (FlyingAudioComponent)
		{
			FlyingAudioComponent->SetActive(true);
		}

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

		// 필요한 경우 추가 초기화 로직
		CurrentPenetrationCount = 0;  // 관통 횟수 초기화
		bIsInPool = false;
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
		bIsInPool = true;

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		// 나이아가라 이펙트 및 사운드 비활성화 (선택 사항)
	}
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

	if (!IsValid(this))
	{
		return;  // PSystem이 유효하지 않거나 이 액터가 이미 파괴된 경우 안전하게 종료
	}

	ReturnToPool();

}

void ADDProjectileBase::StopLifeTimeTimer()
{
	if (!GetWorld())
	{
		return;
	}
	// 타이머가 설정된 경우 타이머 종료
	if (GetWorld()->GetTimerManager().IsTimerActive(LifeSpanTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimerHandle);
	}
}

void ADDProjectileBase::ReturnToPool()
{
	if (!ProjectileManager)
	{
		return;
	}

	if (bIsInPool)
	{
		return;
	}

	if (!IsValid(this))
	{
		return;  // PSystem이 유효하지 않거나 이 액터가 이미 파괴된 경우 안전하게 종료
	}


	// ProjectileManager가 유효한지 확인 후 DestroyProjectile 호출
	if (ProjectileManager && !IsValid(this))
	{
		ProjectileManager->DestroyProjectile(this);
	}
	StopLifeTimeTimer();
}

#pragma endregion Utility
