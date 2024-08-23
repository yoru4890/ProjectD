// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDTowerFactory.h"
#include "LSM/Building/Tower/DDTowerData.h"
#include "LSM/Building/Tower/DDTowerBase.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDBuildingManager.h"

UObject* UDDTowerFactory::CreateObject(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	UDDBuildingManager* BuildingManager = MyGameInstance->GetBuildingManager();
	check(BuildingManager);

	const FDDTowerData* TowerData = BuildingManager->GetTowerData(RowName);
	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(RowName);

	UClass* TowerClass = TowerData->TowerClass;
	check(TowerClass);

	check(World);
	// TrapClass를 사용하여 NewTrap 생성
	ADDTowerBase* NewTower = World->SpawnActor<ADDTowerBase>(TowerClass);
	check(NewTower);

	NewTower->InitFromDataTable(RowName, *TowerData);
	NewTower->SetAssets(BuildingData);
	NewTower->SetActorLocation(Location);
	NewTower->SetActorRotation(Rotation);
	NewTower->SetOwner(Owner);
	NewTower->SetInstigator(Instigator);

	return NewTower;
}
