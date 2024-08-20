// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Animation/DDEnemyAnimInstance.h"
#include "YSY/Enemy/DDEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UDDEnemyAnimInstance::UDDEnemyAnimInstance()
{
}

void UDDEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	owner = Cast<ADDEnemyBase>(GetOwningActor());
	if (owner)
	{
		charMoveComp = owner->GetCharacterMovement();
	}
}

void UDDEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (charMoveComp)
	{
		velocity = charMoveComp->Velocity;
		FVector direction{ owner->GetActorRotation().Vector() };
		FVector rightDirection{ FRotationMatrix(owner->GetActorRotation()).GetScaledAxis(EAxis::Y).GetSafeNormal2D() };
		FVector normalVelocity{ velocity.GetSafeNormal2D() };
		double dotResult{ FVector::DotProduct(direction, normalVelocity) };
		double rightDotResult{ FVector::DotProduct(rightDirection, normalVelocity) };
		double degree{ FMath::RadiansToDegrees(FMath::Acos(dotResult)) };
		if (rightDotResult < 0)
		{
			degree *= -1;
		}

		angle = degree;
		moveSpeed = velocity.Size2D();
	}
}
