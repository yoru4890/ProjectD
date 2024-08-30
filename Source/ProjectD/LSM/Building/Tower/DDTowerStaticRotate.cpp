// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerStaticRotate.h"
#include "LSM/DDRotationComponent.h"
#include "LSM/Building/AttackStrategies/DDBaseAttackStrategy.h"

ADDTowerStaticRotate::ADDTowerStaticRotate()
{
	DDRotationComponent = CreateDefaultSubobject<UDDRotationComponent>(TEXT("DDRotationComponent"));
	RotationSpeed = 10.f;
}

void ADDTowerStaticRotate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateTowardsEnemy();

	if (bIsRecoiling)
	{
		// �ݵ� �ð� ����� ���� �跲 ��ġ�� ���
		RecoilTimeElapsed += DeltaTime;
		float Alpha = FMath::Clamp(RecoilTimeElapsed / MaxRecoilTime, 0.0f, 1.0f);

		// �跲 ��ġ�� �ʱ� ��ġ���� �ݵ� ������ ��ġ�� ����
		FVector NewLocation = FMath::Lerp(InitialBarrelLocation, InitialBarrelLocation + RecoilOffset, Alpha);
		BarrelStaticMeshComponent->SetRelativeLocation(NewLocation);

		// �ݵ��� ������ �ʱ�ȭ
		if (Alpha >= 1.0f)
		{
			bIsRecoiling = false;
			BarrelStaticMeshComponent->SetRelativeLocation(InitialBarrelLocation);
		}
	}

	if (!TargetEnemy)
	{
		SetIsNowAttack(false);
		return;
	}

	if (IsEnemyInSight() && bCanAttack)
	{
		StartAttackProcedure();
		UE_LOG(LogTemp, Warning, TEXT("Tower Tick Test"));
	}
}

void ADDTowerStaticRotate::RotateTowardsEnemy()
{
	if (TargetEnemy)
	{
		// Ÿ�� ���� ���� ȸ��
		DDRotationComponent->RotatePitchAndYawStaticMeshTowardsTarget(BarrelStaticMeshComponent, TargetEnemy, RotationSpeed);
		DDRotationComponent->RotateYawStaticMeshTowardsTarget(WaistMeshComponent, TargetEnemy, RotationSpeed);
	}
}

const bool ADDTowerStaticRotate::IsEnemyInSight(float CosTheta) const
{
	if (!TargetEnemy)
	{
		return false;
	}
	if (!BarrelStaticMeshComponent)
	{
		return false;
	}

	FVector TowerLocation = GetActorLocation();
	TowerLocation.Z = 0;
	FVector EnemyLocation = TargetEnemy->GetActorLocation();
	EnemyLocation.Z = 0;

	FVector FirePointDirection = BarrelStaticMeshComponent->GetSocketRotation(TEXT("FireDirection")).Vector().GetSafeNormal();
	FVector DirectionToEnemy = (EnemyLocation - TowerLocation).GetSafeNormal();

	float DotProduct = FVector::DotProduct(DirectionToEnemy, FirePointDirection);

	return DotProduct >= CosTheta;
}

void ADDTowerStaticRotate::SetIsNowAttack(bool InIsNowAttack)
{
	if (IsNowAttack == true)
	{
		if (InIsNowAttack == false)
		{
			StopAttackEffect();
		}
	}
	IsNowAttack = InIsNowAttack;
}

void ADDTowerStaticRotate::SetMeshs(const FDDBuildingBaseData& LoadedAsset)
{
	Super::SetMeshs(LoadedAsset);

	if (StaticMeshComponents.IsValidIndex(0))
	{
		BarrelStaticMeshComponent = StaticMeshComponents[0];
		BarrelStaticMeshComponent->SetRelativeLocation(BarrelMeshOffset);
	}
	if (StaticMeshComponents.IsValidIndex(1))
	{
		WaistMeshComponent = StaticMeshComponents[1];
		WaistMeshComponent->SetRelativeLocation(WaistMeshOffset);
	}
	if (StaticMeshComponents.IsValidIndex(2))
	{
		PlatformMeshComponent = StaticMeshComponents[2];
	}
}

void ADDTowerStaticRotate::StartRecoil()
{
	if (!bIsRecoiling && BarrelStaticMeshComponent)
	{
		// �跲�� �ʱ� ��ġ�� ����
		InitialBarrelLocation = BarrelStaticMeshComponent->GetRelativeLocation();
		// �ݵ� ���� ����
		bIsRecoiling = true;
		RecoilTimeElapsed = 0.0f;
		// �ݵ� ���� (��: �ڷ� �и���)
		FVector ForwardVector = BarrelStaticMeshComponent->GetForwardVector();
		RecoilOffset = -ForwardVector * RecoilDistance; // �跲�� �ٶ󺸴� ������ �ݴ������� ����
	}
}

void ADDTowerStaticRotate::Attack()
{
	if (AttackStrategy)
	{
		FVector FirePointLocation = BarrelStaticMeshComponent->GetSocketLocation(TEXT("FirePoint"));
		FRotator FirePointDirection = BarrelStaticMeshComponent->GetSocketRotation(TEXT("FirePoint"));

		AttackStrategy->Attack(TargetEnemy, FirePointLocation, FirePointDirection);
	}
	SetIsNowAttack(true);
}

void ADDTowerStaticRotate::StartAttackProcedure()
{
	UE_LOG(LogTemp, Warning, TEXT("StartAttackProcedure"));
	ExecuteAttackEffects();
	if (bCanRecoli)
	{
		StartRecoil();
	}
	Attack();
}
