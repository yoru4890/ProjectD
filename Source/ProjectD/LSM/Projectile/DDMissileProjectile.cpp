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
            SetActorHiddenInGame(true);  // 미사일 숨기기
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
            SetActorHiddenInGame(false);  // 하강할 때 미사일 다시 보이게
            bIsMissileHidden = false;     // 다시 보이는 상태로 플래그 설정
        }
        ApplyDescentAcceleration(DeltaTime);  // 하강 시 가속도 적용
    }
}

void ADDMissileProjectile::SetTargetActor(AActor* InTargetActor)
{
    TargetActor = InTargetActor;
    TargetLocation = InTargetActor->GetActorLocation();
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
    float AscentAcceleration = 10000.0f;  // 상승 가속도
    FVector AccelerationVector = GetActorForwardVector() * AscentAcceleration * DeltaTime;
    ProjectileMovementComponent->Velocity += AccelerationVector;
}

void ADDMissileProjectile::ApplyDescentAcceleration(float DeltaTime)
{
    //float DescentAcceleration = 3000.0f;  // 하강 가속도
    //FVector AccelerationVector = GetActorForwardVector() * DescentAcceleration * DeltaTime;
    //ProjectileMovementComponent->Velocity += AccelerationVector;

    FVector AccelerationVector = GetActorForwardVector() * 10000.f;
    ProjectileMovementComponent->Velocity = AccelerationVector;
}