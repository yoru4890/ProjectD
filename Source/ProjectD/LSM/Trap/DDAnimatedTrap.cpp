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

	USkeletalMeshComponent* FirstSkeletalMeshComponent = nullptr;
	int32 SkeletalNum = 0;


	for (USkeletalMesh* SkeletalMesh : SkeletalMeshs) {
		USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
		check(SkeletalMeshComponent);

		SkeletalMeshComponent->SetupAttachment(RootComponent);
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);

		if (bIsFirstSkeltalMesh) {
			FirstSkeletalMeshComponent = SkeletalMeshComponent;
			if (AnimBlueprint) {
				FirstSkeletalMeshComponent->SetAnimInstanceClass(AnimBlueprint->GeneratedClass);
			}
			FirstSkeletalMeshComponent->RegisterComponent();
			bIsFirstSkeltalMesh = false;
		}
		else {
			SkeletalMeshComponent->SetupAttachment(FirstSkeletalMeshComponent);
			SkeletalMeshComponent->RegisterComponent();
		}
		SkeletalMeshComponents.Add(SkeletalMeshComponent);
		FMaterialsStruct MaterialStruct;
		MaterialStruct.Materials = SkeletalMeshComponent->GetMaterials();
		OriginalMaterials.Add(SkeletalNum, MaterialStruct);
		SkeletalNum++;
		
	}
	FBoxSphereBounds Bounds = FirstSkeletalMeshComponent->GetSkeletalMeshAsset()->GetBounds();
	FVector BoxExtent = Bounds.BoxExtent;
	FVector ScaleFactor = FVector(300.f / (BoxExtent.X * 2 ), 300.f / (BoxExtent.Y * 2 ),1);
	FirstSkeletalMeshComponent->SetWorldScale3D(ScaleFactor);
}
