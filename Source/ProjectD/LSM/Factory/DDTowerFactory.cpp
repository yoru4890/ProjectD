// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDTowerFactory.h"
#include "LSM/Building/Tower/DDTowerData.h"
#include "LSM/Building/Tower/DDTowerBase.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDBuildingManager.h"

UObject* UDDTowerFactory::CreateObject(const FDDFactoryParams& Params)
{
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(Params.World->GetGameInstance());
	check(MyGameInstance);

	UDDBuildingManager* BuildingManager = MyGameInstance->GetBuildingManager();
	check(BuildingManager);

	const FDDTowerData* TowerData = BuildingManager->GetTowerData(Params.RowName);
	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(Params.RowName);

	UClass* TowerClass = TowerData->TowerClass;
	check(TowerClass);

	// TrapClass를 사용하여 NewTrap 생성
	ADDTowerBase* NewTower = Params.World->SpawnActor<ADDTowerBase>(TowerClass);
	check(NewTower);

	NewTower->InitFromDataTable(Params.RowName, *TowerData);
	NewTower->SetAssets(BuildingData);
	NewTower->SetActorLocation(FVector(0, 0, -5000));
	NewTower->SetOwner(Params.Owner);
	NewTower->SetInstigator(Params.Instigator);

	return NewTower;
}
