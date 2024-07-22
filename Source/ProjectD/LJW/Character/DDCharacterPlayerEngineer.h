// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Character/DDCharacterPlayer.h"
#include "DDCharacterPlayerEngineer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDCharacterPlayerEngineer : public ADDCharacterPlayer
{
	GENERATED_BODY()
	
public:
	ADDCharacterPlayerEngineer();

protected:
	virtual void BeginPlay() override;


public :
	//Character Mesh
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	USkeletalMeshComponent* Hair;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	USkeletalMeshComponent* Feet;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	USkeletalMeshComponent* HandL;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	USkeletalMeshComponent* HandR;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	USkeletalMeshComponent* BottomBody;

	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	USkeletalMeshComponent* UpperBody;



	UPROPERTY(VisibleAnywhere, Category = "Rifle")
	class USkeletalMeshComponent* RifleMesh;
};
