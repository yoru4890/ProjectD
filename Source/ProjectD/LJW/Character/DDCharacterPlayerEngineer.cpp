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
}


void ADDCharacterPlayerEngineer::BeginPlay()
{
	Super::BeginPlay();
}



