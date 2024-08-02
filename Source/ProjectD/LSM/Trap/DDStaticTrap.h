// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Trap/DDTrapBase.h"
#include "DDStaticTrap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDStaticTrap : public ADDTrapBase
{
	GENERATED_BODY()

public:
	ADDStaticTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Override to set trap assets
	virtual void SetTrapAssets(FBaseStruct& LoadedAsset) override;

protected:
	// Skeletal Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UStaticMeshComponent>> StaticMeshComponents;

};
