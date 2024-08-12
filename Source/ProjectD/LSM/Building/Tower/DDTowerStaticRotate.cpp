// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerStaticRotate.h"
#include "LSM/DDRotationComponent.h"

ADDTowerStaticRotate::ADDTowerStaticRotate()
{
	DDRotationComponent = CreateDefaultSubobject<UDDRotationComponent>(TEXT("DDRotationComponent"));
}

void ADDTowerStaticRotate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateTowardsEnemy();
}

void ADDTowerStaticRotate::RotateTowardsEnemy()
{
	if (TargetEnemy)
	{
		// 타겟 적을 향해 회전
		DDRotationComponent->RotateStaticMeshTowardsTarget(StaticMeshComponents[0], TargetEnemy, 2.f);
	}
}
