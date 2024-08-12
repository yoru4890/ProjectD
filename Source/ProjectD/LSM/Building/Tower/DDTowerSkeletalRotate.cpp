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
}

void ADDTowerSkeletalRotate::SetTargetEnemy(AActor* NewTargetEnemy)
{
	Super::SetTargetEnemy(NewTargetEnemy);

	if (NewTargetEnemy)
	{
		DDRotationComponent->StartRotateSkeletalMeshTowardsTarget(SkeletalMeshComponents[0], NewTargetEnemy);
	}
	else
	{
		DDRotationComponent->StopRotateSkeletalMeshTowardsTarget(SkeletalMeshComponents[0]);
	}
}
