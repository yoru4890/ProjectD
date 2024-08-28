// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDTrapFactory.h"
#include "LSM/Building/Trap/DDTrap.h"
#include "LSM/Building/Trap/DDTrapData.h"
#include "LSM/Manager/DDAssetManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDBuildingManager.h"

UObject* UDDTrapFactory::CreateObject(UWorld* World, const FName& RowName, AActor* Owner, APawn* Instigator)
{

	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	UDDBuildingManager* BuildingManager = MyGameInstance->GetBuildingManager();
	check(BuildingManager);

	const FDDTrapData* TrapData = BuildingManager->GetTrapData(RowName);
	const FDDBuildingBaseData& BuildingData = *BuildingManager->GetBuildingData(RowName);
	check(TrapData);

	UClass* TrapClass = TrapData->TrapClass;

	check(World);
	check(TrapClass);
	// TrapClass를 사용하여 NewTrap 생성
	ADDTrap* NewTrap = World->SpawnActor<ADDTrap>(TrapClass);
	check(NewTrap);

	NewTrap->InitFromDataTable(RowName, *TrapData);
	NewTrap->SetAssets(BuildingData);
	NewTrap->SetActorLocation(FVector(0,0, -5000));
	NewTrap->SetOwner(Owner);
	NewTrap->SetInstigator(Instigator);
	


	return NewTrap;
}
