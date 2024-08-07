// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDTowerFactory.h"
#include "LSM/Building/Tower/DDTowerData.h"
#include "LSM/Building/Tower/DDTower.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDAssetManager.h"

UObject* UDDTowerFactory::CreateObject(UWorld* World, const FName& RowName, const FDDBuildingBaseData& ObjectStruct, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	check(World);

	const FDDTowerData* TowerStruct = nullptr;
	if (ObjectStruct.BuildingType == EBuildingType::Tower)
	{
		TowerStruct = static_cast<const FDDTowerData*>(&ObjectStruct);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is Not Trap"), *RowName.ToString());
	}

	UClass* TowerClass = TowerStruct->TowerClass;


	check(TowerClass);
	// TrapClass를 사용하여 NewTrap 생성
	ADDTower* NewTower = World->SpawnActor<ADDTower>(TowerClass);
	check(NewTower);

	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	UDDAssetManager* AssetManager = MyGameInstance->GetAssetManager();
	check(AssetManager);

	FDDBuildingBaseData* LoadedAsset = AssetManager->GetLoadedAssetByName(RowName);

	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Loaded At UDDTrapFactory"));
		return nullptr;
	}
	NewTower->InitFromDataTable(RowName, *TowerStruct);
	NewTower->SetAssets(*LoadedAsset);

	NewTower->SetActorLocation(Location);
	NewTower->SetActorRotation(Rotation);
	NewTower->SetOwner(Owner);
	NewTower->SetInstigator(Instigator);

	return NewTower;
}
