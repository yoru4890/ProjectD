// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDTrapFactory.h"
#include "LSM/Building/Trap/DDTrap.h"
#include "LSM/Building/Trap/DDTrapData.h"
#include "LSM/Manager/DDAssetManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDBuildingManager.h"

UObject* UDDTrapFactory::CreateObject(const FDDFactoryParams& Params)
{
	check(Params.World);
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(Params.World->GetGameInstance());
	check(MyGameInstance);

	UDDBuildingManager* BuildingManager = MyGameInstance->GetBuildingManager();
	check(BuildingManager);

	const FDDTrapData* TrapData = BuildingManager->GetTrapData(Params.RowName);
	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(Params.RowName);
	check(TrapData);

	UClass* TrapClass = TrapData->TrapClass;

	check(TrapClass);
	// TrapClass를 사용하여 NewTrap 생성
	ADDTrap* NewTrap = Params.World->SpawnActor<ADDTrap>(TrapClass);
	check(NewTrap);

	NewTrap->InitFromDataTable(Params.RowName, *TrapData);
	NewTrap->SetAssets(BuildingData);
	NewTrap->SetActorLocation(FVector(0,0, -5000));
	NewTrap->SetOwner(Params.Owner);
	NewTrap->SetInstigator(Params.Instigator);
	


	return NewTrap;
}
