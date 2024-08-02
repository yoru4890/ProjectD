// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Trap/DDTrapBase.h"
#include "DDAnimatedTrap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDAnimatedTrap : public ADDTrapBase
{
	GENERATED_BODY()

public:
	ADDAnimatedTrap();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Override to set trap assets
	virtual void SetTrapAssets(FDDBuildingBaseStruct& LoadedAsset) override;

protected:
	// Skeletal Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USkeletalMeshComponent>> SkeletalMeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimMontages", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> AnimMontages;


	virtual void Attack() override;
private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> AttackMontage;
};
