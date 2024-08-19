// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Manager/DDBuildingManager.h"
#include "YSY/Game/DDGameSingleton.h"
#include "YSY/Game/DDGameInstance.h"
#include "DDFactoryManager.h"
#include "DDAssetManager.h"
#include "LSM/Factory/DDFactoryInterface.h"
#include "GameFramework/Actor.h"
#include "YSY/Game/DDPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Building/DDBuildingBase.h"

UDDBuildingManager::UDDBuildingManager()
{

}

void UDDBuildingManager::Initialize()
{
	TMap<FName, FDDTrapData>& TrapDataTable = GetTrapDataTable();
	TMap<FName, FDDTowerData>& TowerDataTable = GetTowerDataTable();

	for (auto& TrapData : TrapDataTable)
	{
		BuildingDataTable.Add(TrapData.Key, &TrapData.Value);
	}

	for (auto& TowerData : TowerDataTable)
	{
		BuildingDataTable.Add(TowerData.Key, &TowerData.Value);
	}

	SetBuildingSellCost();
}

void UDDBuildingManager::SetBuildingSellCost(float Ratio)
{
	for (auto& BuildingDataMap : BuildingDataTable)
	{
		FName BuildingName = BuildingDataMap.Key;
		FDDBuildingBaseData* BuildingData = BuildingDataMap.Value;

		FName ParentName = BuildingData->ParentRowName;
		int32 TotalCost = BuildingData->BuildCost;

		while (ParentName != FName("None"))
		{
			TotalCost += BuildingData->BuildCost;
			ParentName = (*BuildingDataTable.Find(ParentName))->ParentRowName;
		}

		TotalCost = TotalCost * Ratio;

		BuildingData->SellCost = TotalCost;
	}
}

bool UDDBuildingManager::IsBuildingUnlocked(const FName& RowName) const
{
	const FDDBuildingBaseData* BuildingStruct = GetBuildingData(RowName);
	
	bool bIsUnlocked = BuildingStruct->bIsUnlocked;

	return bIsUnlocked;

}

bool UDDBuildingManager::UnlockBuilding(const FName& RowName)
{
	FDDBuildingBaseData* BuildingStruct = GetBuildingData(RowName);

	FName ParentName = BuildingStruct->ParentRowName;

	while (ParentName != FName("None")) {
		if (!GetBuildingData(ParentName)->bIsUnlocked) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Parent Trap is Not Unlocked"));
			return false;
		}
		ParentName = GetBuildingData(ParentName)->ParentRowName;
		
	}
	ADDPlayerState* PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	check(PlayerState);

	if (PlayerState->CheckLikePoint(BuildingStruct->UnlockCost)) {
		PlayerState->SubtractLikePoint(BuildingStruct->UnlockCost);
		BuildingStruct->bIsUnlocked = true;
		UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
		check(MyGameInstance);

		UDDAssetManager* AssetManager = MyGameInstance->GetAssetManager();
		check(AssetManager);
		AssetManager->LoadAssetsAsync(RowName);

		return true;
	}
	return false;
}

bool UDDBuildingManager::LockBuilding(const FName& RowName)
{
	TArray<FName> Stack;
	Stack.Push(RowName);

	while (Stack.Num()>0) {
		FName CurrentName = Stack.Pop();
		FDDBuildingBaseData* CurrentBuildingStruct = GetBuildingData(CurrentName);

		for (auto& ChildName : CurrentBuildingStruct->ChildRowNames)
		{
			if (!GetBuildingData(ChildName)->bIsUnlocked)
			{
				UE_LOG(LogTemp, Warning, TEXT("Child Trap %s is Not Unlocked"), *ChildName.ToString());
				return false;
			}
			Stack.Push(ChildName);
		}
	}
	ADDPlayerState* PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	check(PlayerState);

	FDDBuildingBaseData* LockBuildingData = GetBuildingData(RowName);

	PlayerState->AddLikePoint(LockBuildingData->UnlockCost);
	LockBuildingData->bIsUnlocked = false;

	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	UDDAssetManager* AssetManager = MyGameInstance->GetAssetManager();
	check(AssetManager);
	AssetManager->RemoveLoadedAssetByName(RowName);
	return true;
}

ADDBuildingBase* UDDBuildingManager::SpawnBuilding(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	if (!BuildingPool.Contains(RowName)) {
		BuildingPool.Add(RowName, FBuildingList());
	}

	ADDBuildingBase* NewBuilding = nullptr;

	// 풀에서 트랩을 꺼냅니다.
	if (BuildingPool[RowName].Buildings.Num() > 0) {
		NewBuilding = BuildingPool[RowName].Buildings.Pop();
		if (NewBuilding) {
			// 트랩 위치와 회전을 설정합니다.
			NewBuilding->SetActorLocationAndRotation(Location, Rotation);
		}
	}
	else {
		// World가 null이면 실행 중지
		check(World);

		// 빌딩 데이터 가져오기
		const TMap<FName, FDDBuildingBaseData*>& OutBuildingDataTable = GetBuildingDataTable();

		UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(World->GetGameInstance());
		check(MyGameInstance);

		UDDFactoryManager* FactoryManager = MyGameInstance->GetFactoryManager();
		check(FactoryManager);

		IDDFactoryInterface* BuildingFactory = FactoryManager->GetFactory(RowName);
		check(BuildingFactory);

		UObject* CreatedObject = BuildingFactory->CreateObject(World, RowName, OutBuildingDataTable, Location, Rotation, Owner, Instigator);
		if (!CreatedObject)
		{
			return nullptr;
		}
		NewBuilding = Cast<ADDBuildingBase>(CreatedObject);
	}

	NewBuilding->SetActorHiddenInGame(false);
	NewBuilding->SetActorEnableCollision(true);
	NewBuilding->SetActorTickEnabled(true);
	NewBuilding->SetCanAttack(false);

	return NewBuilding;
}

void UDDBuildingManager::DestroyBuilding(ADDBuildingBase& Building)
{
	BuildingPool[Building.GetRowName()].Buildings.Add(Building);
	Building.SetActorHiddenInGame(true);
	Building.SetActorEnableCollision(false);
	Building.SetActorTickEnabled(false);
	Building.SetCanAttack(false);
}


const FDDBuildingBaseData* UDDBuildingManager::GetBuildingData(const FName& RowName) const
{
	if (!BuildingDataTable.Find(RowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Building data for RowName %s not found!"), *RowName.ToString());
		return nullptr;
	}
	if (const FDDBuildingBaseData* FoundData = *BuildingDataTable.Find(RowName))
	{
		return FoundData;
	}
	// Log an error if the data is not found
	UE_LOG(LogTemp, Warning, TEXT("Building data for RowName %s not found!"), *RowName.ToString());
	
	return nullptr;
}

FDDBuildingBaseData* UDDBuildingManager::GetBuildingData(const FName& RowName)
{
	if (!BuildingDataTable.Find(RowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Building data for RowName %s not found!"), *RowName.ToString());
		return nullptr;
	}
	if (FDDBuildingBaseData* FoundData = *BuildingDataTable.Find(RowName))
	{
		return FoundData;
	}
	// Log an error if the data is not found
	UE_LOG(LogTemp, Warning, TEXT("Building data for RowName %s not found!"), *RowName.ToString());

	return nullptr;

}

const FDDTrapData* UDDBuildingManager::GetTrapData(const FName& RowName) const
{
	const FDDTrapData* FoundData = UDDGameSingleton::Get().GetTrapDataTable().Find(RowName);
	if (!FoundData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trap data for RowName %s not found!"), *RowName.ToString());
	}
	return FoundData;
}

const FDDTowerData* UDDBuildingManager::GetTowerData(const FName& RowName) const
{
	const FDDTowerData* FoundData = UDDGameSingleton::Get().GetTowerDataTable().Find(RowName);
	if (!FoundData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tower data for RowName %s not found!"), *RowName.ToString());
	}
	return FoundData;
}

const TMap<FName, FDDTrapData>& UDDBuildingManager::GetTrapDataTable() const
{
	return UDDGameSingleton::Get().GetTrapDataTable();
}

TMap<FName, FDDTrapData>& UDDBuildingManager::GetTrapDataTable()
{
	return UDDGameSingleton::Get().GetTrapDataTable();
}

const TMap<FName, FDDTowerData>& UDDBuildingManager::GetTowerDataTable() const
{
	return UDDGameSingleton::Get().GetTowerDataTable();
}

TMap<FName, FDDTowerData>& UDDBuildingManager::GetTowerDataTable()
{
	return UDDGameSingleton::Get().GetTowerDataTable();
}

const TMap<FName, FDDBuildingBaseData*>& UDDBuildingManager::GetBuildingDataTable() const
{
	return BuildingDataTable;
}

TMap<FName, FDDBuildingBaseData*>& UDDBuildingManager::GetBuildingDataTable()
{
	return BuildingDataTable;
}
