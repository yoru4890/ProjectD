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

	TimeSinceLastAttack += DeltaTime;

	if (!bCanAttack || !TargetEnemy)
	{
		SetIsNowAttack(false);
		return;
	}
	else
	{
		if (TimeSinceLastAttack >= AttackCoolTime)
		{
			bool bIsEnemyInSight = IsEnemyInSight();
			if (bIsEnemyInSight)
			{
				ExecuteAttackEffects();
				if (AttackStrategy)
				{
					IDDBuildingAttackStrategyInterface* AttackStrategyInterface = Cast<IDDBuildingAttackStrategyInterface>(AttackStrategy);
					AttackStrategyInterface->Attack(TargetEnemy);
				}
				SetIsNowAttack(true);
				TimeSinceLastAttack = 0.f;
			}
			else
			{
				SetIsNowAttack(false);
			}
		}
	}
}

void ADDTowerStaticRotate::RotateTowardsEnemy()
{
	if (TargetEnemy)
	{
		// 타겟 적을 향해 회전
		DDRotationComponent->RotateStaticMeshTowardsTarget(StaticMeshComponents[0], TargetEnemy, RotationSpeed);
	}
}

const bool ADDTowerStaticRotate::IsEnemyInSight(float CosTheta) const
{
	if (!TargetEnemy)
	{
		return false;
	}
	if (StaticMeshComponents.Num() == 0 || !StaticMeshComponents[0])
	{
		return false;
	}

	FVector TowerLocation = GetActorLocation();
	TowerLocation.Z = 0;
	FVector EnemyLocation = TargetEnemy->GetActorLocation();
	EnemyLocation.Z = 0;

	FVector FirePointDirection = StaticMeshComponents[0]->GetSocketRotation(TEXT("FirePoint1")).Vector().GetSafeNormal();
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
