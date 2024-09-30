// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Projectile/DDMissileProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "YSY/Collision/CollisionChannel.h"
#include "Components/AudioComponent.h"

void ADDMissileProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bApexHeightForRotate)
    {
        // 고도에 도달했는지 체크 및 가속도 적용
        ApplyAscentAcceleration(DeltaTime);

        // 미사일이 ApexHeightForVisible에 도달하면 숨기기
        if (!bIsMissileHidden && GetActorLocation().Z >= ApexHeightForVisible)
        {
            StaticMeshComponent->SetVisibility(false);
            bIsMissileHidden = true;     // 숨김 상태 플래그 설정
        }

        // ApexHeightForRotate에 도달하면 회전 시작
        if (GetActorLocation().Z >= ApexHeightForRotate)
        {
            ReachApexAndStartDescent();
        }
    }
    else if (bIsDescending)
    {
        // 미사일이 다시 ApexHeightForVisible 이하로 내려오면 보이게
        if (bIsMissileHidden && GetActorLocation().Z <= ApexHeightForVisible)
        {
            StaticMeshComponent->SetVisibility(true);
            bIsMissileHidden = false;     // 다시 보이는 상태로 플래그 설정
        }
        ApplyDescentAcceleration(DeltaTime);  // 하강 시 가속도 적용
    }
}

void ADDMissileProjectile::SetTargetActor(AActor* InTargetActor)
{
    TargetActor = InTargetActor;
    FVector RandomOffset = FVector(FMath::RandRange(-100.0f, 100.0f), FMath::RandRange(-100.0f, 100.0f), 0.0f);
    TargetLocation = InTargetActor->GetActorLocation() + RandomOffset;

    AscentAcceleration = 10000.f + FMath::RandRange(-2000.f, 2000.f);
    DescentAcceleration = 10000.f + FMath::RandRange(-2000.f, 2000.f);
}

void ADDMissileProjectile::LaunchProjectile()
{
    bTrailEffectFinished = true;
    bExplosionEffectFinished = true;
    bApexHeightForRotate = false;
    bIsDescending = false;
    bIsMissileHidden = false;  // 미사일 숨김 상태 초기화
    TargetActor = nullptr;

    TrailNiagaraComponent->SetActive(true);
    bTrailEffectFinished = false;

    Super::LaunchProjectile();
}

void ADDMissileProjectile::SetAssetAndManager(const FDDProjectileData& LoadedAsset, UDDProjectileManager* InProjectileManager)
{
    Super::SetAssetAndManager(LoadedAsset, InProjectileManager);
    SetAttachNiagaraComponent();

}

void ADDMissileProjectile::SetAttachNiagaraComponent()
{
    USceneComponent* TargetComponent = nullptr;
    FName TrailSocketName = FName("TrailPoint");
    FName FireSocketName = FName("FirePoint");
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

        TrailNiagaraComponent->OnSystemFinished.AddDynamic(this, &ADDMissileProjectile::OnTrailEffectFinished);
    }

    TargetComponent = nullptr;
    if (StaticMeshComponent->DoesSocketExist(FireSocketName))
    {
        TargetComponent = StaticMeshComponent;
    }

    if (TargetComponent && ExplosionEffect)
    {
        UNiagaraComponent* NewNiagaraComponent = NewObject<UNiagaraComponent>(this);
        ExplosionNiagaraComponent = NewNiagaraComponent;
        ExplosionNiagaraComponent->SetAsset(ExplosionEffect);
        ExplosionNiagaraComponent->SetupAttachment(TargetComponent, FireSocketName);
        ExplosionNiagaraComponent->SetAutoActivate(false);  // 기본적으로 비활성화
        ExplosionNiagaraComponent->RegisterComponent();

        ExplosionNiagaraComponent->OnSystemFinished.AddDynamic(this, &ADDMissileProjectile::OnExplosionEffectFinished);
    }
}

void ADDMissileProjectile::ReachApexAndStartDescent()
{
    bApexHeightForRotate = true;
    bIsDescending = true;

    // 적을 향해 회전
    TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
    SetActorRotation(TargetRotation);
    ProjectileMovementComponent->StopMovementImmediately();
}

void ADDMissileProjectile::ApplyAscentAcceleration(float DeltaTime)
{
    FVector AccelerationVector = GetActorForwardVector() * AscentAcceleration * DeltaTime;
    ProjectileMovementComponent->Velocity += AccelerationVector;
}

void ADDMissileProjectile::ApplyDescentAcceleration(float DeltaTime)
{
    FVector AccelerationVector = GetActorForwardVector() * DescentAcceleration * DeltaTime;
    ProjectileMovementComponent->Velocity += AccelerationVector;
}

void ADDMissileProjectile::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnCollisionBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    ProjectileMovementComponent->SetActive(false);
    StaticMeshComponent->SetVisibility(false);
    if (FlyingAudioComponent)
    {
        FlyingAudioComponent->Stop();
    }

    if (ExplosionNiagaraComponent)
    {
        ExplosionNiagaraComponent->SetActive(true);
        bExplosionEffectFinished = false;
    }

    if (TrailNiagaraComponent)
    {
        TrailNiagaraComponent->Deactivate();
    }
    ApplyRadialDamage();
    SetActorEnableCollision(false);
}

void ADDMissileProjectile::ApplyRadialDamage()
{
    // 폭발 범위 내에 있는 적들만 찾아 데미지를 적용
    TArray<AActor*> OverlappedActors;

    // 적들이 속하는 오브젝트 타입 (ECC_Pawn을 사용해 적을 찾는다고 가정)
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(GTCHANNEL_ENEMY)); // 적이 ECC_Pawn 채널에 있을 경우

    // SphereOverlapActors를 사용해 폭발 범위 내의 적 찾기
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        GetActorLocation(),      // 폭발 중심
        ExplosionRadius,         // 폭발 반경
        ObjectTypes,             // Pawn 채널에 속하는 적들만 탐색
        nullptr,                 // 특정 클래스의 액터만 찾고 싶으면 지정 (여기서는 생략)
        TArray<AActor*>(),       // 제외할 액터 목록 (여기서는 생략)
        OverlappedActors         // 결과로 얻은 적들
    );

    // 찾은 액터들에게 데미지 적용 (IDamageInterface를 구현한 경우에만)
    for (AActor* Actor : OverlappedActors)
    {
        ApplyDamageToActor(Actor);
    }

    UE_LOG(LogTemp, Warning, TEXT("Explosion applied to %d enemies"), OverlappedActors.Num());
}

void ADDMissileProjectile::OnTrailEffectFinished(UNiagaraComponent* PSystem)
{
    bTrailEffectFinished = true;
    HandleEffectCompletion();
}

void ADDMissileProjectile::OnExplosionEffectFinished(UNiagaraComponent* PSystem)
{
    bExplosionEffectFinished = true;
    HandleEffectCompletion();
}

void ADDMissileProjectile::HandleEffectCompletion()
{
    if (bTrailEffectFinished && bExplosionEffectFinished)
    {
        ReturnToPool();
    }
}
