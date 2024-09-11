// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Projectile/DDMissileProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
    bApexHeightForRotate = false;
    bIsDescending = false;
    bIsMissileHidden = false;  // �̻��� ���� ���� �ʱ�ȭ
    TargetActor = nullptr;
    Super::LaunchProjectile();
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