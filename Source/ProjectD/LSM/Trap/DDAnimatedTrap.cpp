// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Trap/DDAnimatedTrap.h"

ADDAnimatedTrap::ADDAnimatedTrap()
{

}

void ADDAnimatedTrap::BeginPlay()
{
	Super::BeginPlay();
}

void ADDAnimatedTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADDAnimatedTrap::SetTrapAssets(TArray<UStaticMesh*> StaticMeshs, TArray<USkeletalMesh*> SkeletalMeshs, UAnimBlueprint* AnimBlueprint, TArray<UParticleSystem*> ParticleEffects)
{
	Super::SetTrapAssets(StaticMeshs, SkeletalMeshs, AnimBlueprint, ParticleEffects);
	// 기존 ParticleEffectComponents 배열 초기화
	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		if (SkeletalMeshComponent)
		{
			SkeletalMeshComponent->DestroyComponent();
		}
	}
	SkeletalMeshComponents.Empty();
	bool bIsFirstSkeltalMesh = true;

	for (USkeletalMesh* SkeletalMesh : SkeletalMeshs) {
		USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
		check(SkeletalMeshComponent);

		SkeletalMeshComponent->SetupAttachment(RootComponent);
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);

		if (bIsFirstSkeltalMesh) {
			SetRootComponent(SkeletalMeshComponent);
			if (AnimBlueprint) {
				SkeletalMeshComponent->SetAnimInstanceClass(AnimBlueprint->GeneratedClass);
			}
			bIsFirstSkeltalMesh = false;
		}
		SkeletalMeshComponent->RegisterComponent();
		SkeletalMeshComponents.Add(SkeletalMeshComponent);
	}
}
