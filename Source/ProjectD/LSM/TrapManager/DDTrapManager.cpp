// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/TrapManager/DDTrapManager.h"
#include "YSY/Game/DDGameSingleton.h"
#include "YSY/Game/DDGameInstance.h"
#include "DDTrapFactoryManager.h"
#include "DDTrapAssetManager.h"
#include "LSM/Trap/DDTrapBase.h"
#include "GameFramework/Actor.h"

UDDTrapManager::UDDTrapManager()
{

}

void UDDTrapManager::SetTrapFactoryManager(UDDTrapFactoryManager* InTrapFactoryManager)
{
	TrapFactoryManager = InTrapFactoryManager;
}


bool UDDTrapManager::IsTowerUnlocked(const FName& TrapName) const
{
	const FDDTrapStruct* TrapStruct = GetTrapData(TrapName);
	check(TrapStruct);

	return TrapStruct->bIsTrapUnlocked;

}

void UDDTrapManager::UnlockTower(const FName& TrapName)
{
	FDDTrapStruct* TrapStruct = GetTrapData(TrapName);
	TrapStruct->bIsTrapUnlocked = true;

}

const FDDTrapStruct* UDDTrapManager::GetTrapData(const FName& TrapName) const
{
	return UDDGameSingleton::Get().GetTrapDataTable().Find(TrapName);
}

FDDTrapStruct* UDDTrapManager::GetTrapData(const FName& TrapName)
{
	return UDDGameSingleton::Get().GetTrapDataTable().Find(TrapName);
}

TMap<FName, FDDTrapStruct>& UDDTrapManager::GetTrapDataTable()
{
	return UDDGameSingleton::Get().GetTrapDataTable();
}

const TMap<FName, FDDTrapStruct>& UDDTrapManager::GetTrapDataTable() const
{
	return UDDGameSingleton::Get().GetTrapDataTable();
}

ADDTrapBase* UDDTrapManager::SpawnTrap(UWorld* World, const FName& TrapName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	check(World); // World가 null이면 실행 중지

	const FDDTrapStruct* TrapStruct = GetTrapData(TrapName);
	check(TrapStruct); // TrapStruct가 null이면 실행 중지

	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	UDDTrapFactoryManager* FactoryManager = MyGameInstance->GetTrapFactoryManager();
	check(FactoryManager);

	IDDTrapFactoryInterface* TrapFactory = FactoryManager->GetTrapFactory(TrapName);
	check(TrapFactory);

	TrapFactory->CreateTrap(World, TrapName, Location, Rotation, Owner, Instigator);

	ADDTrapBase* NewTrap = Cast<ADDTrapBase>(TrapFactory->CreateTrap(World, TrapName, Location, Rotation, Owner, Instigator));
	if (NewTrap)
	{
		// 트랩 초기화 함수 호출
		NewTrap->InitFromDataTable(*UDDTrapManager::GetTrapData(TrapName));

		UStaticMesh* StaticMesh = MyGameInstance->GetTrapAssetManager()->GetStaticMesh(TrapName);
		USkeletalMesh* SkeletalMesh = MyGameInstance->GetTrapAssetManager()->GetSkeletalMesh(TrapName);
		UAnimBlueprint* AnimBlueprint = MyGameInstance->GetTrapAssetManager()->GetAnimBlueprint(TrapName);
		UParticleSystem* ParticleEffect = MyGameInstance->GetTrapAssetManager()->GetParticleEffect(TrapName);

		ITrapAssetInterface* TrapAssetInterface = Cast<ITrapAssetInterface>(NewTrap);
		if (TrapAssetInterface)
		{
			TrapAssetInterface->SetTrapAssets(StaticMesh, SkeletalMesh, AnimBlueprint, ParticleEffect);
		}

	}

	return NewTrap;

}
