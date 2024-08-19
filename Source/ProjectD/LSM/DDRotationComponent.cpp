// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/DDRotationComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "LSM/Building/DDBuildingAnimInstance.h"

// Sets default values for this component's properties
UDDRotationComponent::UDDRotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDDRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDDRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDDRotationComponent::RotateStaticMeshTowardsTarget(UStaticMeshComponent* MeshComponent, const AActor* Target, const float RotationSpeed) const
{
	if (!MeshComponent || !Target)
	{
		return;
	}

	FRotator TargetRotation;
	FVector TargetLocation = Target->GetActorLocation();
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	TargetRotation = Direction.Rotation();

	// Pitch�� Roll�� 0���� �����ϰ�, Yaw�� ����
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;

	FRotator CurrentRotation = MeshComponent->GetRelativeRotation();

	// CurrentRotation�� Pitch�� Roll�� 0���� �����ϰ�, Yaw�� ����
	CurrentRotation.Pitch = 0.0f;
	CurrentRotation.Roll = 0.0f;

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);

	// ���Ͱ� �ƴ� �޽� ������Ʈ�� ȸ���� ����
	MeshComponent->SetWorldRotation(NewRotation);
}

void UDDRotationComponent::StartRotateSkeletalMeshTowardsTarget(USkeletalMeshComponent* MeshComponent, AActor* Target) const
{
	if (!MeshComponent || !Target)
	{
		return;
	}

	// Target ��ġ�κ��� ��ǥ ȸ�� ���
	FVector TargetLocation = Target->GetActorLocation();

	// �ִϸ��̼� �ν��Ͻ��� ���� �� ȸ�� ����
	if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
	{
		UDDBuildingAnimInstance* BuildingAnimInstance = Cast<UDDBuildingAnimInstance>(AnimInstance);

		if (BuildingAnimInstance)
		{
			BuildingAnimInstance->SetIsRotateToTarget(true);
			BuildingAnimInstance->SetTargetActor(Target);
		}
	}
}

void UDDRotationComponent::StopRotateSkeletalMeshTowardsTarget(USkeletalMeshComponent* MeshComponent)
{
	// �ִϸ��̼� �ν��Ͻ��� ���� �� ȸ�� ����
	if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
	{
		UDDBuildingAnimInstance* BuildingAnimInstance = Cast<UDDBuildingAnimInstance>(AnimInstance);

		if (BuildingAnimInstance)
		{
			BuildingAnimInstance->SetIsRotateToTarget(false);
			BuildingAnimInstance->SetTargetActor(nullptr);
		}
	}
}

