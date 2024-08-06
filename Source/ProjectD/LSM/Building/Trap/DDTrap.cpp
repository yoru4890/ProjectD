// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Trap/DDTrap.h"
#include "LSM/Building/Trap/DDTrapAnimInstance.h"

void ADDTrap::BeginPlay()
{
	Super::BeginPlay();
}

void ADDTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADDTrap::Attack()
{
	Super::Attack();

	if (AttackMontages.IsEmpty() || SkeletalMeshComponents.IsEmpty())
	{
		return;
	}

	for (int index = 0; index < SkeletalMeshComponents.Num(); index++)
	{
		UAnimInstance* AnimInstance = SkeletalMeshComponents[index]->GetAnimInstance();
		if (AnimInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnimInstance Exist"));
			// UDDTrapAnimInstance로 캐스팅합니다.
			UDDTrapAnimInstance* TrapAnimInstance = Cast<UDDTrapAnimInstance>(AnimInstance);
			if (TrapAnimInstance && AttackMontages.IsValidIndex(index))
			{
				UE_LOG(LogTemp, Warning, TEXT("Trap AnimInstance Exist"));
				// PlayAnimationMontage를 호출합니다.
				TrapAnimInstance->PlayAnimationMontage(AttackMontages[index]);
			}
		}
	}
}
