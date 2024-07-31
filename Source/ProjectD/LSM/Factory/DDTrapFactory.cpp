// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDTrapFactory.h"
#include "LSM/Trap/DDTrapBase.h"
#include "LSM/Trap/DDTrapStruct.h"
#include "LSM/Manager/DDAssetManager.h"
#include "YSY/Game/DDGameInstance.h"

UObject* UDDTrapFactory::CreateObject(UWorld* World, const FName& RowName, const FBaseStruct& ObjectStruct, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	check(World);
	
	const FDDTrapStruct* TrapStruct = static_cast<const FDDTrapStruct*>(&ObjectStruct);
	UClass* TrapClass = TrapStruct->TrapClass;

	
	check(TrapClass);
	// TrapClass를 사용하여 NewTrap 생성
	ADDTrapBase* NewTrap = World->SpawnActor<ADDTrapBase>(TrapClass);
	check(NewTrap);

	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	UDDAssetManager* AssetManager = MyGameInstance->GetAssetManager();
	check(AssetManager);

	FBaseStruct* LoadedAsset = AssetManager->GetLoadedAssetByName(RowName);

	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Loaded At UDDTrapFactory"));
		return nullptr;
	}

	NewTrap->SetTrapAssets(*LoadedAsset);

	NewTrap->SetActorLocation(Location);
	NewTrap->SetActorRotation(Rotation);
	NewTrap->SetOwner(Owner);
	NewTrap->SetInstigator(Instigator);
	NewTrap->InitFromDataTable(RowName,*TrapStruct);
	


	return NewTrap;
}
