// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Trap/DDStaticTrap.h"

ADDStaticTrap::ADDStaticTrap()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
}

void ADDStaticTrap::BeginPlay()
{
}

void ADDStaticTrap::Tick(float DeltaTime)
{
}

void ADDStaticTrap::SetTrapAssets(UStaticMesh* StaticMesh, USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBlueprint, UParticleSystem* ParticleEffect)
{
	Super::SetTrapAssets(StaticMesh, SkeletalMesh, AnimBlueprint, ParticleEffect);
	if (StaticMeshComponent && StaticMesh) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}
}
