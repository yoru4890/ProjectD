// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerSkeletalRotate.h"
#include "LSM/DDRotationComponent.h"
#include "NiagaraComponent.h"
#include "LSM/Building/AttackStrategies/DDBaseAttackStrategy.h"

ADDTowerSkeletalRotate::ADDTowerSkeletalRotate()
{
	DDRotationComponent = CreateDefaultSubobject<UDDRotationComponent>(TEXT("DDRotationComponent"));
}

void ADDTowerSkeletalRotate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSinceLastAttack += DeltaTime;

	if (!bCanAttack || !TargetEnemy)
	{
		//SetIsNowAttack(false);
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
					//AttackStrategy->Attack(TargetEnemy);
				}
				//SetIsNowAttack(true);
				TimeSinceLastAttack = 0.f;
			}
			else
			{
				//SetIsNowAttack(false);
			}
		}
	}
}

void ADDTowerSkeletalRotate::SetTargetEnemy(AActor* NewTargetEnemy)
{
	Super::SetTargetEnemy(NewTargetEnemy);
	if (SkeletalMeshComponents.Num() == 0 || !SkeletalMeshComponents[0])
	{
		return;
	}
	if (NewTargetEnemy)
	{
		DDRotationComponent->StartRotateSkeletalMeshTowardsTarget(SkeletalMeshComponents[0], NewTargetEnemy);
	}
	else
	{
		DDRotationComponent->StopRotateSkeletalMeshTowardsTarget(SkeletalMeshComponents[0]);
	}
}

const bool ADDTowerSkeletalRotate::IsEnemyInSight(float CosTheta) const
{
	if (!TargetEnemy)
	{
		return false;
	}
	if (SkeletalMeshComponents.Num() == 0 || !SkeletalMeshComponents[0])
	{
		return false;
	}

	FVector TowerLocation = GetActorLocation();
	TowerLocation.Z = 0;
	FVector EnemyLocation = TargetEnemy->GetActorLocation();
	EnemyLocation.Z = 0;
	
	FVector FirePointDirection = SkeletalMeshComponents[0]->GetSocketRotation(TEXT("FirePoint")).Vector().GetSafeNormal();
	FVector DirectionToEnemy = (EnemyLocation - TowerLocation).GetSafeNormal();

	float DotProduct = FVector::DotProduct(DirectionToEnemy, FirePointDirection);

	return DotProduct >= CosTheta;

}

void ADDTowerSkeletalRotate::SetIsNowAttack(bool InIsNowAttack)
{
	if (IsNowAttack == true)
	{
		if (InIsNowAttack == false)
		{
			//StopAttackEffect();
		}
	}
	IsNowAttack = InIsNowAttack;
}
