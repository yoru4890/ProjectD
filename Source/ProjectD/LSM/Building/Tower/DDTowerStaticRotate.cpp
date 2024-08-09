// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerStaticRotate.h"
#include "LSM/DDRotationComponent.h"

ADDTowerStaticRotate::ADDTowerStaticRotate()
{
	DDRotationComponent = CreateDefaultSubobject<UDDRotationComponent>(TEXT("DDRotationComponent"));
}

void ADDTowerStaticRotate::SetCanAttack(bool bIsCanAttack)
{
	Super::SetCanAttack(bIsCanAttack);

	if (bIsCanAttack)
	{
		GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &ADDTowerStaticRotate::RotateTowardsEnemy, 0.5f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
	}
}

void ADDTowerStaticRotate::RotateTowardsEnemy()
{
	if (TargetEnemy)
	{
		// Ÿ�� ���� ���� ȸ��
		DDRotationComponent->RotateStaticMeshTowardsTarget(StaticMeshComponents[0], TargetEnemy, 10.f);
	}
	else
	{
		// Ÿ���� ������ٸ� Ÿ�̸Ӹ� ����
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
	}
}
