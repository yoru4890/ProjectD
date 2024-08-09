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

	FRotator CurrentRotation = MeshComponent->GetRelativeRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);

	// 액터가 아닌 메쉬 컴포넌트에 회전을 적용
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

//void UDDRotationComponent::RotateTowards(const AActor* Target, const float RotationSpeed, const bool bIsRotateBone = false, const FName BoneName = NAME_None) const
//{
//	if (!Target)
//	{
//		return;
//	}
//
//    FRotator TargetRotation;
//    FVector TargetLocation = Target->GetActorLocation();
//    FVector CurrentLocation = GetOwner()->GetActorLocation();
//    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
//    TargetRotation = Direction.Rotation();
//
//    if (!bIsRotateBone)
//    {
//        FRotator CurrentRotation = GetOwner()->GetActorRotation();
//        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
//        GetOwner()->SetActorRotation(NewRotation);
//    }
//    else
//    {
//        USkeletalMeshComponent* SkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
//        if (SkeletalMeshComponent && SkeletalMeshComponent->DoesSocketExist(BoneName))
//        {
//            FRotator CurrentBoneRotation = SkeletalMeshComponent->GetSocketRotation(BoneName);
//            FRotator NewBoneRotation = FMath::RInterpTo(CurrentBoneRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
//            SkeletalMeshComponent->SetBoneRotationByName(BoneName, NewBoneRotation, EBoneSpaces::ComponentSpace);
//        }
//    }
//}

