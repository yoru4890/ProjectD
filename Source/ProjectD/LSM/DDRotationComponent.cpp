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

void UDDRotationComponent::RotateYawStaticMeshTowardsTarget(UStaticMeshComponent* MeshComponent, const AActor* Target, const float RotationSpeed) const
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

	// Pitch와 Roll을 0으로 설정하고, Yaw만 유지
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;

	FRotator CurrentRotation = MeshComponent->GetComponentRotation();

	// CurrentRotation의 Pitch와 Roll을 0으로 설정하고, Yaw만 유지
	CurrentRotation.Pitch = 0.0f;
	CurrentRotation.Roll = 0.0f;

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);

	// 액터가 아닌 메쉬 컴포넌트에 회전을 적용
	MeshComponent->SetWorldRotation(NewRotation);
}

void UDDRotationComponent::RotatePitchAndYawStaticMeshTowardsTarget(UStaticMeshComponent* MeshComponent, const AActor* Target, const float RotationSpeed) const
{
	if (!MeshComponent || !Target)
	{
		return;
	}



	// FirePoint 소켓의 위치를 얻습니다.
	FVector FirePointLocation = MeshComponent->GetSocketLocation(FName("FireDirection"));
	FVector TargetLocation = Target->GetActorLocation();
	FVector Direction = (TargetLocation - FirePointLocation).GetSafeNormal();

	float DistanceToTarget = FVector::Dist(FirePointLocation, TargetLocation);

	// 타겟을 향한 회전 값을 계산합니다.
	FRotator TargetRotation = Direction.Rotation();
	TargetRotation.Roll = 0.0f; // Roll 값을 0으로 설정하여 Yaw와 Pitch만 유지합니다.

	// 현재 메쉬 컴포넌트의 회전을 얻습니다.
	FRotator CurrentRotation = MeshComponent->GetComponentRotation();
	CurrentRotation.Roll = 0.0f; // Roll 값을 0으로 설정하여 Yaw와 Pitch만 유지합니다.

	// 메쉬 컴포넌트의 회전을 타겟 회전 값으로 보간하여 새 회전을 계산합니다.
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);

	// 회전 각도 제한 (예: 45도 이상 회전하지 않도록 제한)
	const float MaxPitchRotation = 20.0f;

	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -MaxPitchRotation, MaxPitchRotation);

	// 메쉬 컴포넌트에 새로운 회전 값을 적용합니다.
	MeshComponent->SetWorldRotation(NewRotation);
}

void UDDRotationComponent::StartRotateSkeletalMeshTowardsTarget(USkeletalMeshComponent* MeshComponent, AActor* Target) const
{
	if (!MeshComponent || !Target)
	{
		return;
	}

	// Target 위치로부터 목표 회전 계산
	FVector TargetLocation = Target->GetActorLocation();

	// 애니메이션 인스턴스를 통해 본 회전 적용
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
	// 애니메이션 인스턴스를 통해 본 회전 적용
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

