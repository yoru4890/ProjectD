// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Trap/DDAnimatedTrap.h"

ADDAnimatedTrap::ADDAnimatedTrap()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMeshComponent;
}

void ADDAnimatedTrap::BeginPlay()
{
}

void ADDAnimatedTrap::Tick(float DeltaTime)
{
}

void ADDAnimatedTrap::SetTrapAssets(UStaticMesh* StaticMesh, USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBlueprint, UParticleSystem* ParticleEffect)
{
	Super::SetTrapAssets(StaticMesh, SkeletalMesh, AnimBlueprint, ParticleEffect);
	if (SkeletalMeshComponent && SkeletalMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
		if (AnimBlueprint)
		{
			SkeletalMeshComponent->SetAnimInstanceClass(AnimBlueprint->GeneratedClass);
		}
	}
}
