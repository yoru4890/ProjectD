// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDTrapFactory.h"
#include "LSM/Building/Trap/DDTrapBase.h"
#include "LSM/Building/Trap/DDTrapData.h"
#include "LSM/Manager/DDAssetManager.h"
#include "YSY/Game/DDGameInstance.h"

UObject* UDDTrapFactory::CreateObject(UWorld* World, const FName& RowName, const FDDBuildingBaseData& ObjectStruct, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	check(World);
	
	const FDDTrapData* TrapStruct = static_cast<const FDDTrapData*>(&ObjectStruct);
	UClass* TrapClass = TrapStruct->TrapClass;

	
	check(TrapClass);
	// TrapClass를 사용하여 NewTrap 생성
	ADDTrapBase* NewTrap = World->SpawnActor<ADDTrapBase>(TrapClass);
	check(NewTrap);

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
	NewTrap->InitFromDataTable(RowName, *TrapStruct);
	NewTrap->SetAssets(*LoadedAsset);

	NewTrap->SetActorLocation(Location);
	NewTrap->SetActorRotation(Rotation);
	NewTrap->SetOwner(Owner);
	NewTrap->SetInstigator(Instigator);
	


	return NewTrap;
}
