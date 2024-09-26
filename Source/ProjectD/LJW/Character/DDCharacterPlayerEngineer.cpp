// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Character/DDCharacterPlayerEngineer.h"

ADDCharacterPlayerEngineer::ADDCharacterPlayerEngineer() 
{

	//Skeletal Mesh
	// Head
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_HeadMorphs.SK_HeadMorphs'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	//Hair
	CreateLeaderPoseSkeletalMesh(Hair, TEXT("Hair"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_FHair.SK_FHair'"));
	
	//Feet
	CreateLeaderPoseSkeletalMesh(Feet, TEXT("Feet"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_Shoes.SK_Shoes'"));
	

	//Hand_L
	CreateLeaderPoseSkeletalMesh(HandL, TEXT("Hand_L"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_Hand_L.SK_Hand_L'"));
	
	//Hand_R
	CreateLeaderPoseSkeletalMesh(HandR, TEXT("Hand_R"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_Hand_R.SK_Hand_R'"));
	
	//UpperBody
	CreateLeaderPoseSkeletalMesh(UpperBody, TEXT("UpperBody"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_UpperBody_C.SK_UpperBody_C'"));
	
	//BottomBody
	CreateLeaderPoseSkeletalMesh(BottomBody, TEXT("BottomBody"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_BottomBody_B.SK_BottomBody_B'"));

	//ABP
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/0000/LJW/Animation/ABP_PlayerEngineer.ABP_PlayerEngineer_C'"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	//Animation Tick Option
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	//Materials
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HitEffectMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/0000/LSM/Material/MI_HoloFX_02.MI_HoloFX_02'"));
	if (HitEffectMaterialRef.Object)
	{
		HitEffectMaterial=HitEffectMaterialRef.Object;
	}
}


void ADDCharacterPlayerEngineer::BeginPlay()
{
	Super::BeginPlay();
	PopulateMaterials(Hair, HairMaterials);
	PopulateMaterials(Feet, FeetMaterials);
	PopulateMaterials(HandL, HandLMaterials);
	PopulateMaterials(HandR, HandRMaterials);
	PopulateMaterials(UpperBody, UpperBodyMaterials);
	PopulateMaterials(BottomBody, BottomBodyMaterials);
}

void ADDCharacterPlayerEngineer::ApplyHitMaterial()
{
	Super::ApplyHitMaterial();
	UE_LOG(LogTemp, Warning, TEXT("Character Apply Hit Material"));
	SetMaterialForMesh(Hair, HitEffectMaterial);
	SetMaterialForMesh(Feet, HitEffectMaterial);
	SetMaterialForMesh(HandL, HitEffectMaterial);
	SetMaterialForMesh(HandR, HitEffectMaterial);
	SetMaterialForMesh(UpperBody, HitEffectMaterial);
	SetMaterialForMesh(BottomBody, HitEffectMaterial);
}

void ADDCharacterPlayerEngineer::RestoreOriginalMaterials()
{
	Super::RestoreOriginalMaterials();
	UE_LOG(LogTemp, Warning, TEXT("Character Restore Original Material"));
	RestoreMaterialsForMesh(Hair, HairMaterials);
	RestoreMaterialsForMesh(Feet, FeetMaterials);
	RestoreMaterialsForMesh(HandL, HandLMaterials);
	RestoreMaterialsForMesh(HandR, HandRMaterials);
	RestoreMaterialsForMesh(UpperBody, UpperBodyMaterials);
	RestoreMaterialsForMesh(BottomBody, BottomBodyMaterials);

}

