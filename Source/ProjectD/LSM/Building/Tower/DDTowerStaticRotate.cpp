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
		// 반동 시간 경과에 따라 배럴 위치를 계산
		RecoilTimeElapsed += DeltaTime;
		float Alpha = FMath::Clamp(RecoilTimeElapsed / MaxRecoilTime, 0.0f, 1.0f);

		// 배럴 위치를 초기 위치에서 반동 오프셋 위치로 보간
		FVector NewLocation = FMath::Lerp(InitialBarrelLocation, InitialBarrelLocation + RecoilOffset, Alpha);
		BarrelStaticMeshComponent->SetRelativeLocation(NewLocation);

		// 반동이 끝나면 초기화
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
		// 타겟 적을 향해 회전
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
		// 배럴의 초기 위치를 저장
		InitialBarrelLocation = BarrelStaticMeshComponent->GetRelativeLocation();
		// 반동 상태 시작
		bIsRecoiling = true;
		RecoilTimeElapsed = 0.0f;
		// 반동 방향 (예: 뒤로 밀리기)
		FVector ForwardVector = BarrelStaticMeshComponent->GetForwardVector();
		RecoilOffset = -ForwardVector * RecoilDistance; // 배럴이 바라보는 방향의 반대쪽으로 설정
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
