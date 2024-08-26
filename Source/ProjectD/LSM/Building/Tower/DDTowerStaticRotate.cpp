// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerStaticRotate.h"
#include "LSM/DDRotationComponent.h"
#include "LSM/Building/AttackStrategies/DDBuildingBaseAttackStrategy.h"

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
			UE_LOG(LogTemp, Warning, TEXT("Check"));
			bool bIsEnemyInSight = IsEnemyInSight();
			if (bIsEnemyInSight)
			{
				UE_LOG(LogTemp, Warning, TEXT("Check1"));
				ExecuteAttackEffects();
				if (AttackStrategy)
				{
					AttackStrategy->Attack(TargetEnemy);
				}
				SetIsNowAttack(true);
				TimeSinceLastAttack = 0.f;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Check2"));
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
