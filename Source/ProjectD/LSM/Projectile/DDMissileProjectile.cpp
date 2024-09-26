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
        // ���� �����ߴ��� üũ �� ���ӵ� ����
        ApplyAscentAcceleration(DeltaTime);

        // �̻����� ApexHeightForVisible�� �����ϸ� �����
        if (!bIsMissileHidden && GetActorLocation().Z >= ApexHeightForVisible)
        {
            StaticMeshComponent->SetVisibility(false);
            bIsMissileHidden = true;     // ���� ���� �÷��� ����
        }

        // ApexHeightForRotate�� �����ϸ� ȸ�� ����
        if (GetActorLocation().Z >= ApexHeightForRotate)
        {
            ReachApexAndStartDescent();
        }
    }
    else if (bIsDescending)
    {
        // �̻����� �ٽ� ApexHeightForVisible ���Ϸ� �������� ���̰�
        if (bIsMissileHidden && GetActorLocation().Z <= ApexHeightForVisible)
        {
            StaticMeshComponent->SetVisibility(true);
            bIsMissileHidden = false;     // �ٽ� ���̴� ���·� �÷��� ����
        }
        ApplyDescentAcceleration(DeltaTime);  // �ϰ� �� ���ӵ� ����
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
    bIsMissileHidden = false;  // �̻��� ���� ���� �ʱ�ȭ
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
        TrailNiagaraComponent->SetAutoActivate(false);  // �⺻������ ��Ȱ��ȭ
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
        ExplosionNiagaraComponent->SetAutoActivate(false);  // �⺻������ ��Ȱ��ȭ
        ExplosionNiagaraComponent->RegisterComponent();

        ExplosionNiagaraComponent->OnSystemFinished.AddDynamic(this, &ADDMissileProjectile::OnExplosionEffectFinished);
    }
}

void ADDMissileProjectile::ReachApexAndStartDescent()
{
    bApexHeightForRotate = true;
    bIsDescending = true;

    // ���� ���� ȸ��
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
    // ���� ���� ���� �ִ� ���鸸 ã�� �������� ����
    TArray<AActor*> OverlappedActors;

    // ������ ���ϴ� ������Ʈ Ÿ�� (ECC_Pawn�� ����� ���� ã�´ٰ� ����)
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(GTCHANNEL_ENEMY)); // ���� ECC_Pawn ä�ο� ���� ���

    // SphereOverlapActors�� ����� ���� ���� ���� �� ã��
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        GetActorLocation(),      // ���� �߽�
        ExplosionRadius,         // ���� �ݰ�
        ObjectTypes,             // Pawn ä�ο� ���ϴ� ���鸸 Ž��
        nullptr,                 // Ư�� Ŭ������ ���͸� ã�� ������ ���� (���⼭�� ����)
        TArray<AActor*>(),       // ������ ���� ��� (���⼭�� ����)
        OverlappedActors         // ����� ���� ����
    );

    // ã�� ���͵鿡�� ������ ���� (IDamageInterface�� ������ ��쿡��)
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
