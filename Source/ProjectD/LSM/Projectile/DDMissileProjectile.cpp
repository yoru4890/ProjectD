// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Projectile/DDMissileProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
    bApexHeightForRotate = false;
    bIsDescending = false;
    bIsMissileHidden = false;  // 미사일 숨김 상태 초기화
    TargetActor = nullptr;
    Super::LaunchProjectile();
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