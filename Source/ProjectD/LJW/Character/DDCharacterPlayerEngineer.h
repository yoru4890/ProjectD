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
	//Mesh
	//Character
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	TObjectPtr<USkeletalMeshComponent> Hair;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	TObjectPtr<USkeletalMeshComponent> Feet;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	TObjectPtr<USkeletalMeshComponent> HandL;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	TObjectPtr<USkeletalMeshComponent> HandR;
	
	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	TObjectPtr<USkeletalMeshComponent> BottomBody;

	UPROPERTY(VisibleAnywhere, Category = "CharacterMesh")
	TObjectPtr<USkeletalMeshComponent> UpperBody;

private:
	// Material
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> HairMaterials;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> FeetMaterials;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> HandLMaterials;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> HandRMaterials;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> BottomBodyMaterials;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> UpperBodyMaterials;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInterface> HitEffectMaterial;


	virtual void ApplyHitMaterial() override;

	virtual void RestoreOriginalMaterials() override;


};
