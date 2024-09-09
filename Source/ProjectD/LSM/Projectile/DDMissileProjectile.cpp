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
            SetActorHiddenInGame(true);  // �̻��� �����
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
            SetActorHiddenInGame(false);  // �ϰ��� �� �̻��� �ٽ� ���̰�
            bIsMissileHidden = false;     // �ٽ� ���̴� ���·� �÷��� ����
        }
        ApplyDescentAcceleration(DeltaTime);  // �ϰ� �� ���ӵ� ����
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
    float AscentAcceleration = 10000.0f;  // ��� ���ӵ�
    FVector AccelerationVector = GetActorForwardVector() * AscentAcceleration * DeltaTime;
    ProjectileMovementComponent->Velocity += AccelerationVector;
}

void ADDMissileProjectile::ApplyDescentAcceleration(float DeltaTime)
{
    //float DescentAcceleration = 3000.0f;  // �ϰ� ���ӵ�
    //FVector AccelerationVector = GetActorForwardVector() * DescentAcceleration * DeltaTime;
    //ProjectileMovementComponent->Velocity += AccelerationVector;

    FVector AccelerationVector = GetActorForwardVector() * 10000.f;
    ProjectileMovementComponent->Velocity = AccelerationVector;
}