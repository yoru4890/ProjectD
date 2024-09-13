// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Projectile/DDShellProjectile.h"
#include "NiagaraComponent.h"

void ADDShellProjectile::SetAssetAndManager(const FDDProjectileData& LoadedAsset, UDDProjectileManager* InProjectileManager)
{
	Super::SetAssetAndManager(LoadedAsset, InProjectileManager);
	SetAttachNiagaraComponent();
}

void ADDShellProjectile::SetAttachNiagaraComponent()
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

        TrailNiagaraComponent->OnSystemFinished.AddDynamic(this, &ADDShellProjectile::OnTrailEffectFinished);
    }
    TargetComponent = nullptr;

    if (StaticMeshComponent->DoesSocketExist(FireSocketName))
    {
        TargetComponent = StaticMeshComponent;
    }

    if (ImpactEffect)
    {
        UNiagaraComponent* NewNiagaraComponent = NewObject<UNiagaraComponent>(this);
        ImpactNiagaraComponent = NewNiagaraComponent;
        ImpactNiagaraComponent->SetAsset(ImpactEffect);
        ImpactNiagaraComponent->SetupAttachment(TargetComponent,FireSocketName);
        ImpactNiagaraComponent->SetAutoActivate(false);  // 기본적으로 비활성화
        ImpactNiagaraComponent->RegisterComponent();

        ImpactNiagaraComponent->OnSystemFinished.AddDynamic(this, &ADDShellProjectile::OnImpactEffectFinished);
    }
}

void ADDShellProjectile::LaunchProjectile()
{
    bTrailEffectFinished = true;
    bImpactEffectFinished = true;

    if (TrailNiagaraComponent)
    {
        TrailNiagaraComponent->SetActive(true);
    }
    bTrailEffectFinished = false;
    DamagedActors.Empty();
    Super::LaunchProjectile();
}

void ADDShellProjectile::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnCollisionBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    if (ImpactNiagaraComponent)
    {
        if (ImpactNiagaraComponent->IsActive())
        {
            ImpactNiagaraComponent->Deactivate();
        }
        ImpactNiagaraComponent->SetActive(true);
        bImpactEffectFinished = false;
        UE_LOG(LogTemp, Warning, TEXT("Impact Test"));
    }

    if (OtherActor && !DamagedActors.Contains(OtherActor))
    {
        ApplyDamageToActor(OtherActor);
        DamagedActors.Add(OtherActor);
    }

    if (CurrentPenetrationCount >= MaxPenetrationCount)
    {
        StaticMeshComponent->SetVisibility(false);

        
        if (TrailNiagaraComponent)
        {
            TrailNiagaraComponent->Deactivate();
        }

        SetActorEnableCollision(false);
    }
}

void ADDShellProjectile::OnTrailEffectFinished(UNiagaraComponent* PSystem)
{
    bTrailEffectFinished = true;
    HandleEffectCompletion();
}

void ADDShellProjectile::OnImpactEffectFinished(UNiagaraComponent* PSystem)
{
    bImpactEffectFinished = true;
    HandleEffectCompletion();
}

void ADDShellProjectile::HandleEffectCompletion()
{
    if (bTrailEffectFinished && bImpactEffectFinished)
    {
        ReturnToPool();
    }
}
