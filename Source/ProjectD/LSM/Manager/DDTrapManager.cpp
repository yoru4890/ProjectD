// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Manager/DDTrapManager.h"
#include "YSY/Game/DDGameSingleton.h"
#include "YSY/Game/DDGameInstance.h"
#include "DDFactoryManager.h"
#include "DDAssetManager.h"
#include "LSM/Trap/DDTrapBase.h"
#include "LSM/Factory/DDFactoryInterface.h"
#include "GameFramework/Actor.h"
#include "YSY/Game/DDPlayerState.h"
#include "Kismet/GameplayStatics.h"

UDDTrapManager::UDDTrapManager()
{

}

bool UDDTrapManager::IsTowerUnlocked(const FName& RowName) const
{
	const FDDTrapStruct& TrapStruct = GetTrapData(RowName);

	return TrapStruct.bIsTrapUnlocked;

}

bool UDDTrapManager::UnlockTower(const FName& RowName)
{
	FDDTrapStruct& TrapStruct = GetTrapData(RowName);

	FName ParentName = TrapStruct.TrapParentRowName;

	while (ParentName != FName("None")) {
		if (!GetTrapData(ParentName).bIsTrapUnlocked) 
		{
			return false;
		}
		ParentName = GetTrapData(ParentName).TrapParentRowName;
		
	}
	ADDPlayerState* PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	check(PlayerState);

	if (PlayerState->CheckLikePoint(TrapStruct.TrapUnlockCost)) {
		PlayerState->SubtractLikePoint(TrapStruct.TrapUnlockCost);
		return true;
	}

	return false;
}

const FDDTrapStruct& UDDTrapManager::GetTrapData(const FName& RowName) const
{
	return *UDDGameSingleton::Get().GetTrapDataTable().Find(RowName);
}

FDDTrapStruct& UDDTrapManager::GetTrapData(const FName& RowName)
{
	return *UDDGameSingleton::Get().GetTrapDataTable().Find(RowName);
}

TMap<FName, FDDTrapStruct>& UDDTrapManager::GetTrapDataTable()
{
	return UDDGameSingleton::Get().GetTrapDataTable();
}

const TMap<FName, FDDTrapStruct>& UDDTrapManager::GetTrapDataTable() const
{
	return UDDGameSingleton::Get().GetTrapDataTable();
}

ADDTrapBase* UDDTrapManager::SpawnTrap(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	if (!TrapPool.Contains(RowName)) {
		TrapPool.Add(RowName, FTrapList());
	}

	ADDTrapBase* NewTrap = nullptr;

	// 풀에서 트랩을 꺼냅니다.
	if (TrapPool[RowName].Traps.Num() > 0) {
		NewTrap = TrapPool[RowName].Traps.Pop();
		if (NewTrap) {
			// 트랩 위치와 회전을 설정합니다.
			NewTrap->SetActorLocationAndRotation(Location, Rotation);
		}
	}
	else {
		// World가 null이면 실행 중지
		check(World);

		// 트랩 데이터를 가져옵니다.
		const FDDTrapStruct& TrapStruct = GetTrapData(RowName);

		UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(World->GetGameInstance());
		check(MyGameInstance);

		UDDFactoryManager* FactoryManager = MyGameInstance->GetFactoryManager();
		check(FactoryManager);

		IDDFactoryInterface* TrapFactory = FactoryManager->GetFactory(RowName);
		check(TrapFactory);

		UObject* CreatedObject = TrapFactory->CreateObject(World, RowName, TrapStruct, Location, Rotation, Owner, Instigator);
		NewTrap = Cast<ADDTrapBase>(CreatedObject);
	}

	NewTrap->SetActorHiddenInGame(false);
	NewTrap->SetActorEnableCollision(true);
	NewTrap->SetActorTickEnabled(true);
	NewTrap->SetTrapCanAttack(false);

	return NewTrap;
}

void UDDTrapManager::DestroyTrap(ADDTrapBase& Trap)
{
	TrapPool[Trap.GetTrapRowName()].Traps.Add(Trap);
	Trap.SetActorHiddenInGame(true);
	Trap.SetActorEnableCollision(false);
	Trap.SetActorTickEnabled(false);
	Trap.SetTrapCanAttack(false);
}
