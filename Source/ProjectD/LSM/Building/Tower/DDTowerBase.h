// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Building/DDBuildingBase.h"
#include "DDTowerBase.generated.h"

UCLASS()
class PROJECTD_API ADDTowerBase : public ADDBuildingBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDTowerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Attack() override;
	virtual void SetTargetEnemy(AActor* NewTargetEnemy);

private:
	virtual void ModifyMeshAndAttackCollision() const override;
	virtual void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	AActor* GetNearstEnemy();

public:
	virtual void InitFromDataTable(const FName& InRowName, const FDDBuildingBaseData& BuildingData);

	virtual void SetAssets(FDDBuildingBaseData& LoadedAsset) override;

protected:
	float TowerAttackRange;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetEnemy;

};