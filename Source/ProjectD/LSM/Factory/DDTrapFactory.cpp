// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Factory/DDTrapFactory.h"
#include "LSM/Trap/DDTrapBase.h"
#include "LSM/Trap/DDTrapStruct.h"
#include "LSM/Manager/DDAssetManager.h"
#include "YSY/Game/DDGameInstance.h"

UObject* UDDTrapFactory::CreateObject(UWorld* World, const FName& ObjectName, const FBaseStruct& ObjectStruct, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
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

	FLoadedAsset& LoadedAsset = AssetManager->GetLoadedAssetByName(ObjectName);

	NewTrap->SetTrapAssets(LoadedAsset.StaticMeshs, LoadedAsset.SkeletalMeshs, LoadedAsset.AnimBlueprint, LoadedAsset.Effects);
	NewTrap->SetAttachParticleToRoot();

	NewTrap->SetActorLocationAndRotation(Location, Rotation);
	NewTrap->SetOwner(Owner);
	NewTrap->SetInstigator(Instigator);
	NewTrap->InitFromDataTable(*TrapStruct);


	return NewTrap;
}
