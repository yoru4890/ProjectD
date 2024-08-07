// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDTowerData.h"
#include "DDTowerBase.generated.h"

UCLASS()
class PROJECTD_API ADDTowerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDTowerBase();
	virtual ~ADDTowerBase() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


//public:
//	FORCEINLINE const FName& GetTowerRowName() const { return TowerRowName; }
//	FORCEINLINE const int32 GetTowerCellWidth() const { return TowerCellWidth; }
//
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
//	virtual void InitFromDataTable(const FName& RowName, const FDDTowerData& TowerData);
//	void  SetTowerCanAttack(const bool bInCanAttack);
//	virtual void SetTowerAssets(FDDBuildingBaseData& LoadedAsset) override;
//	void SetMaterialToPreview(bool bCanPay);
//	void SetMaterialToOriginal();

};