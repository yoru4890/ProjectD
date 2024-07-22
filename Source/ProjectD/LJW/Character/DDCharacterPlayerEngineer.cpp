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
	CreateSkeletalMesh(Hair, TEXT("Hair"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_FHair.SK_FHair'"));
	
	//Feet
	CreateSkeletalMesh(Feet, TEXT("Feet"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_Shoes.SK_Shoes'"));
	
	//Hand_L
	CreateSkeletalMesh(HandL, TEXT("Hand_L"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_Hand_L.SK_Hand_L'"));
	
	//Hand_R
	CreateSkeletalMesh(HandR, TEXT("Hand_R"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_Hand_R.SK_Hand_R'"));
	
	//UpperBody
	CreateSkeletalMesh(UpperBody, TEXT("UpperBody"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_UpperBody_C.SK_UpperBody_C'"));
	
	//BottomBody
	CreateSkeletalMesh(BottomBody, TEXT("BottomBody"), TEXT("/Script/Engine.SkeletalMesh'/Game/G2_Mercenaries/Meshes/Characters/Seperates/Females/SK_BottomBody_B.SK_BottomBody_B'"));

	//ABP
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT(""));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}


}

void ADDCharacterPlayerEngineer::BeginPlay()
{
	Super::BeginPlay();
}

