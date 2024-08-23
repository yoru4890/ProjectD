// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/AttackStrategies/DDBuildingBaseAttackStrategy.h"
#include "Engine/DamageEvents.h"
#include "LSM/Building/DDBuildingBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "YSY/Interface/DamageInterface.h"


void UDDBuildingBaseAttackStrategy::Initialize(ADDBuildingBase* InOwningTower)
{
	OwningTower = InOwningTower;
	DamageType = InOwningTower->GetDamageType();
	Damage = InOwningTower->GetDamage();
	DotDamage = InOwningTower->GetDamage();
	DotDuration = InOwningTower->GetDamage();
	DotInterval = InOwningTower->GetDamage();
	SlowAmount = InOwningTower->GetDamage();
	SlowDuration = InOwningTower->GetDamage();
}


void UDDBuildingBaseAttackStrategy::Attack(AActor* TargetEnemy)
{
	

}

void UDDBuildingBaseAttackStrategy::ApplyDotDamge(AActor* TargetEnemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("Apply Dot Damage"));
}

void UDDBuildingBaseAttackStrategy::ApplyDirectDamage(AActor* TargetEnemy)
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

void UDDBuildingBaseAttackStrategy::ApplySlowEffect(AActor* TargetEnemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("Apply Slow Effect"));
}

void UDDBuildingBaseAttackStrategy::PlayHitEffect(AActor* TargetEnemy, FVector Location, FRotator Rotation, FName SocketName, bool bAttachToTarget)
{
    UNiagaraSystem* NiagaraEffect = OwningTower->GetHitEffect();

    if (!OwningTower || !NiagaraEffect || !TargetEnemy)
    {
        return;
    }
    FVector Origin, BoxExtent;
    TargetEnemy->GetActorBounds(true, Origin, BoxExtent);

    // Ÿ������ �������� ���� ���� ���
    FVector Direction = (Origin - OwningTower->GetActorLocation()).GetSafeNormal();

    FVector EffectRotation = -Direction;

    // ���� Bounds �ܰ����� Ÿ���� ���ϴ� ���͸� ���� ���� ����� �� ���
    FVector ClosestPoint = Origin - (Direction * (BoxExtent/2));

    // ���� ������ ����
    FVector RandomOffset;
    RandomOffset.Z = FMath::FRandRange(-20.f, 20.f);

    ClosestPoint += RandomOffset;

    if (bAttachToTarget && TargetEnemy)
    {
        // ��Ʈ ����Ʈ�� ������ ����
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            NiagaraEffect,
            ClosestPoint,
            EffectRotation.Rotation()
        );
    }
    else
    {
        // ��Ʈ ����Ʈ�� Ư�� ��ġ�� ���� (���� ������ ȿ�� ��)
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            NiagaraEffect,
            Location,
            Rotation
        );
    }
}
