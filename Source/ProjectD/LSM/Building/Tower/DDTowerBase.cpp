// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Building/Tower/DDTowerBase.h"
#include "Components/SphereComponent.h"
#include "DDTowerData.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADDTowerBase::ADDTowerBase()
{
	AttackCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollisionComponent"));
	AttackCollisionComponent->SetupAttachment(RootComponent);
	SetupAttackCollisionResponses();
}

void ADDTowerBase::BeginPlay()
{
	Super::BeginPlay();
}

void ADDTowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADDTowerBase::Attack()
{
	Super::Attack();
}

void ADDTowerBase::SetTargetEnemy(AActor* NewTargetEnemy)
{
	TargetEnemy = NewTargetEnemy;
}

void ADDTowerBase::ModifyMeshAndAttackCollision() const
{
	Super::ModifyMeshAndAttackCollision();
	Cast<USphereComponent>(AttackCollisionComponent)->SetSphereRadius(TowerAttackRange);
}

void ADDTowerBase::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxCollisionBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (!TargetEnemy)
	{
		SetTargetEnemy(OtherActor);
	}
}

void ADDTowerBase::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnBoxCollisionEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (TargetEnemy && TargetEnemy == OtherActor)
	{
		SetTargetEnemy(GetNearstEnemy());
	}

}

AActor* ADDTowerBase::GetNearstEnemy()
{
	AActor* NearestEnemy = nullptr;
	float NearestDistance = TNumericLimits<float>::Max();
	for (AActor* Enemies : EnemiesInRanged)
	{
		for (AActor* Enemy : EnemiesInRanged)
		{
			if (Enemy)
			{
				// 적과의 거리를 계산
				float Distance = FVector::Dist(this->GetActorLocation(), Enemy->GetActorLocation());

				// 계산된 거리가 현재까지 찾은 가장 가까운 거리보다 짧은지 확인
				if (Distance < NearestDistance)
				{
					NearestDistance = Distance;
					NearestEnemy = Enemy;
				}
			}
		}
	}
	return NearestEnemy;
}

void ADDTowerBase::SetAssets(FDDBuildingBaseData& LoadedAsset)
{
	Super::SetAssets(LoadedAsset);
}

void ADDTowerBase::InitFromDataTable(const FName& InRowName, const FDDBuildingBaseData& BuildingData)
{
	Super::InitFromDataTable(InRowName, BuildingData);

	const FDDTowerData* TowerData = nullptr;
	if (BuildingData.BuildingType == EBuildingType::Tower)
	{
		TowerData = static_cast<const FDDTowerData*>(&BuildingData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is Not Tower"), *RowName.ToString());
	}

	this->TowerAttackRange = TowerData->TowerAttackRange;
}

