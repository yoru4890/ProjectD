// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Trap/DDStaticTrap.h"

ADDStaticTrap::ADDStaticTrap()
{

}

void ADDStaticTrap::BeginPlay()
{
	Super::BeginPlay();
}

void ADDStaticTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADDStaticTrap::SetTrapAssets(TArray<UStaticMesh*> StaticMeshs, TArray<USkeletalMesh*> SkeletalMeshs, UAnimBlueprint* AnimBlueprint, TArray<UParticleSystem*> ParticleEffects)
{
	Super::SetTrapAssets(StaticMeshs, SkeletalMeshs, AnimBlueprint, ParticleEffects);
	// 기존 ParticleEffectComponents 배열 초기화
	for (UStaticMeshComponent* StaticMeshComponent : StaticMeshComponents)
	{
		if (StaticMeshComponent)
		{
			StaticMeshComponent->DestroyComponent();
		}
	}
	StaticMeshComponents.Empty();
	bool bIsFirstStaticMesh = true;

	for (UStaticMesh* StaticMesh : StaticMeshs) {
		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		check(StaticMeshComponent);

		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetupAttachment(RootComponent);

		if (bIsFirstStaticMesh) {
			SetRootComponent(StaticMeshComponent);
			bIsFirstStaticMesh = false;
		}

		StaticMeshComponent->RegisterComponent();
		StaticMeshComponents.Add(StaticMeshComponent);
	}
}

//void ADDStaticTrap::SetTrapAssets(UStaticMesh* StaticMesh, USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBlueprint, TArray<UParticleSystem*> ParticleEffects)
//{
//	//Super::SetTrapAssets(StaticMesh, SkeletalMesh, AnimBlueprint, ParticleEffect);
//	//if (StaticMeshComponent && StaticMesh) {
//	//	StaticMeshComponent->SetStaticMesh(StaticMesh);
//	//}
//}
