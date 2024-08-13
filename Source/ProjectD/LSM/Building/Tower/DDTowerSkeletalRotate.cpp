// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerSkeletalRotate.h"
#include "LSM/DDRotationComponent.h"

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
		return;
	}
	else
	{
		if (TimeSinceLastAttack >= AttackCoolTime && IsEnemyInSight())
		{
			Attack();
			TimeSinceLastAttack = 0.f;
			UE_LOG(LogTemp, Warning, TEXT("Attack"));
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


	UE_LOG(LogTemp, Warning, TEXT("DotProduct : %f"),DotProduct);

	return DotProduct >= CosTheta;

}
