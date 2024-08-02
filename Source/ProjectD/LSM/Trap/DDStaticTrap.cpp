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

void ADDStaticTrap::SetTrapAssets(FDDBuildingBaseStruct& LoadedAsset)
{
	Super::SetTrapAssets(LoadedAsset);
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

	UStaticMeshComponent* FirstStaticMeshComponent = nullptr;
	int32 StaticNum = 0;

	for (TSoftObjectPtr<UStaticMesh>& StaticMeshSoftPtr : LoadedAsset.StaticMeshs) {
		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		check(StaticMeshComponent);

		if (StaticMeshSoftPtr.IsValid())
		{
			StaticMeshComponent->SetStaticMesh(StaticMeshSoftPtr.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : StaticMesh not loaded"), TrapRowName);
		}
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMeshComponent->SetupAttachment(RootComponent);

		if (bIsFirstStaticMesh) {
			FirstStaticMeshComponent = StaticMeshComponent;
			bIsFirstStaticMesh = false;
		}
		else {
			StaticMeshComponent->SetupAttachment(FirstStaticMeshComponent);
		}

		StaticMeshComponent->RegisterComponent();
		StaticMeshComponents.Add(StaticMeshComponent);

		FMaterialsStruct MaterialStruct;
		MaterialStruct.Materials = StaticMeshComponent->GetMaterials();
		OriginalMaterials.Add(StaticNum, MaterialStruct);
		StaticNum++;
	}

	FBoxSphereBounds Bounds = FirstStaticMeshComponent->GetStaticMesh()->GetBounds();
	FirstStaticMeshComponent->SetRelativeLocation(FVector(0, 0, TrapMeshZAxisModify));
	FVector BoxExtent = Bounds.BoxExtent;
	FVector ScaleFactor = FVector(GridCellSize * TrapCellWidth / (BoxExtent.X * 2), GridCellSize * TrapCellWidth / (BoxExtent.Y * 2), GridCellSize * TrapCellWidth / (BoxExtent.X * 2));
	FirstStaticMeshComponent->SetWorldScale3D(ScaleFactor);
}
