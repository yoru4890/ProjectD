// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Trap/DDAnimatedTrap.h"
#include "LSM/Trap/DDTrapAnimInstance.h"

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

void ADDAnimatedTrap::SetTrapAssets(FDDBuildingBaseStruct& LoadedAsset)
{
	Super::SetTrapAssets(LoadedAsset);
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

	for (TSoftObjectPtr<UAnimMontage>& AnimMontageSoftPtr : LoadedAsset.AnimMontages)
	{
		if (AnimMontageSoftPtr.IsValid())
		{
			AnimMontages.Add(AnimMontageSoftPtr.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : Montages not loaded"), TrapRowName);
		}
	}
	for (TSoftObjectPtr<USkeletalMesh>& SkeletalMeshSoftPtr : LoadedAsset.SkeletalMeshs) {
		USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
		check(SkeletalMeshComponent);

		SkeletalMeshComponent->SetupAttachment(RootComponent);
		if (SkeletalMeshSoftPtr.IsValid()) 
		{
			SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshSoftPtr.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : SkeletalMesh not loaded"), TrapRowName);
		}

		if (bIsFirstSkeltalMesh) {
			FirstSkeletalMeshComponent = SkeletalMeshComponent;
			if (LoadedAsset.MyAnimBlueprint.IsValid())
			{
				FirstSkeletalMeshComponent->SetAnimInstanceClass(LoadedAsset.MyAnimBlueprint->GeneratedClass);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s : AnimBluePrint not loaded"), TrapRowName);
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
	USkeletalMesh* SkeletalMesh = FirstSkeletalMeshComponent->GetSkeletalMeshAsset();
	if (!SkeletalMesh)
	{
		return;
	}

	FBoxSphereBounds Bounds = SkeletalMesh->GetBounds();
	FVector BoxExtent = Bounds.BoxExtent;
	FVector ScaleFactor = FVector(GridCellSize*TrapCellWidth / (BoxExtent.X * 2 ), GridCellSize * TrapCellWidth / (BoxExtent.Y * 2 ), GridCellSize * TrapCellWidth / (BoxExtent.X * 2));
	FirstSkeletalMeshComponent->SetWorldScale3D(ScaleFactor);
	FirstSkeletalMeshComponent->SetRelativeLocation(FVector(0, 0, TrapMeshZAxisModify));
	UE_LOG(LogTemp, Warning, TEXT("TrapMeshZAxisModify is : %f"), TrapMeshZAxisModify);

	if (!AnimMontages.IsEmpty()) 
	{
		AttackMontage = AnimMontages[0];
	}
}

void ADDAnimatedTrap::Attack()
{
	Super::Attack();

	UAnimInstance* AnimInstance = SkeletalMeshComponents[0]->GetAnimInstance();
	if (AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance Exist"));
		// UDDTrapAnimInstance로 캐스팅합니다.
		UDDTrapAnimInstance* TrapAnimInstance = Cast<UDDTrapAnimInstance>(AnimInstance);
		if (TrapAnimInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Trap AnimInstance Exist"));
			// PlayAnimationMontage를 호출합니다.
			TrapAnimInstance->PlayAnimationMontage(AttackMontage);
		}
	}


}
