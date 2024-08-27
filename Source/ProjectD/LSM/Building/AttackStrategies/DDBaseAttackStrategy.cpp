// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDBaseAttackStrategy.h"
#include "Engine/DamageEvents.h"
#include "LSM/Building/DDBuildingBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "YSY/Interface/DamageInterface.h"


void UDDBaseAttackStrategy::Initialize(ADDBuildingBase* InOwningTower)
{
	OwningTower = InOwningTower;
	DamageType = InOwningTower->GetDamageType();
	Damage = InOwningTower->GetDamage();
	DotDamage = InOwningTower->GetDotDamage();
	DotDuration = InOwningTower->GetDotDuration();
	DotInterval = InOwningTower->GetDotInterval();
	SlowAmount = InOwningTower->GetSlowAmount();
	SlowDuration = InOwningTower->GeSlowDuration();
}


void UDDBaseAttackStrategy::Attack(AActor* TargetEnemy, const FVector& FireLocation, const FRotator& FireRotation)
{
	

}

void UDDBaseAttackStrategy::ApplyDotDamge(AActor* TargetEnemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("Apply Dot Damage"));
}

void UDDBaseAttackStrategy::ApplyDirectDamage(AActor* TargetEnemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("Apply Direct Damage"));
    IDamageInterface* DamageInterface = Cast<IDamageInterface>(TargetEnemy);
    if (DamageInterface)
    {
        FDamageEvent DamageEvent{};
        DamageEvent.DamageTypeClass = DamageType;
        DamageInterface->ApplyDamage(Damage, DamageEvent, nullptr, OwningTower);

        UE_LOG(LogTemp, Warning, TEXT("Apply Direct Damage"));
    }
}

void UDDBaseAttackStrategy::ApplySlowEffect(AActor* TargetEnemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("Apply Slow Effect"));
}

void UDDBaseAttackStrategy::PlayHitEffect(AActor* TargetEnemy, FVector Location, FRotator Rotation, FName SocketName, bool bAttachToTarget)
{
    UNiagaraSystem* NiagaraEffect = OwningTower->GetHitEffect();

    if (!OwningTower || !NiagaraEffect || !TargetEnemy)
    {
        return;
    }
    FVector Origin, BoxExtent;
    TargetEnemy->GetActorBounds(true, Origin, BoxExtent);

    // 타워에서 적까지의 방향 벡터 계산
    FVector Direction = (Origin - OwningTower->GetActorLocation()).GetSafeNormal();

    FVector EffectRotation = -Direction;

    // 적의 Bounds 외곽에서 타워를 향하는 벡터를 따라 가장 가까운 점 계산
    FVector ClosestPoint = Origin - (Direction * (BoxExtent/2));

    // 랜덤 오프셋 생성
    FVector RandomOffset;
    RandomOffset.Z = FMath::FRandRange(-20.f, 20.f);

    ClosestPoint += RandomOffset;

    if (bAttachToTarget && TargetEnemy)
    {
        // 히트 이펙트를 적에게 생성
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            NiagaraEffect,
            ClosestPoint,
            EffectRotation.Rotation()
        );
    }
    else
    {
        // 히트 이펙트를 특정 위치에 생성 (땅에 터지는 효과 등)
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            NiagaraEffect,
            Location,
            Rotation
        );
    }
}