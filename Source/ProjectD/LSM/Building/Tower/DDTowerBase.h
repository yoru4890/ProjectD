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
	virtual void SetTargetEnemy(AActor* NewTargetEnemy);

private:
	virtual void ModifyMeshAndAttackCollision() const override;
	virtual void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	AActor* GetNearstEnemy();

public:
	virtual void InitFromDataTable(const FName& InRowName, const FDDBuildingBaseData& BuildingData);

protected:
	float TowerAttackRange;

	FVector InitialBarrelLocation; // 배럴의 초기 위치
	FVector RecoilOffset; // 반동으로 인해 이동할 위치
	bool bCanRecoli = false;
	bool bIsRecoiling = false; // 반동 중인지 여부
	float RecoilTimeElapsed = 0.0f; // 반동 시간 경과
	float MaxRecoilTime = 0.2f; // 반동이 지속되는 시간 (초)
	float RecoilDistance = 10.0f; // 반동 거리

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetEnemy;

};