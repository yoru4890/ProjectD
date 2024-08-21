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
	SetupCommonReferences(GetWorld());
	InitializeBuildings();
}

void UDDBuildingManager::SetupCommonReferences(UWorld* World)
{
	MyGameInstance = Cast<UDDGameInstance>(World->GetGameInstance());
	check(MyGameInstance);

	AssetManager = MyGameInstance->GetAssetManager();
	check(AssetManager);

	FactoryManager = MyGameInstance->GetFactoryManager();
	check(FactoryManager);
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

void UDDBuildingManager::InitializeBuildings()
{
	for (auto& Elem : BuildingDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is Unlock? : %s"), *Elem.Key.ToString(), Elem.Value->bIsUnlocked ? TEXT("true") : TEXT("false"));

		if (Elem.Value->bIsUnlocked)
		{
			TArray<TSoftObjectPtr<UObject>> AssetsToLoad;
			// 에셋 매니저에게 로딩 요청
			GetSoftObjectPtrsInBuilding(Elem.Key, AssetsToLoad);
			if (AssetManager)
			{
				AssetManager->LoadAssetsAsync(AssetsToLoad); // 에셋 리스트 전달
			}

			UE_LOG(LogTemp, Warning, TEXT("%s : Init"), *Elem.Key.ToString());
		}
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
		TArray<TSoftObjectPtr<UObject>> AssetsToload;
		GetSoftObjectPtrsInBuilding(RowName, AssetsToload);
		AssetManager->LoadAssetsAsync(AssetsToload);

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

	FDDBuildingBaseData* LockBuildingData = GetBuildingData(RowName);

	ADDPlayerState* PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	check(PlayerState);

	PlayerState->AddLikePoint(LockBuildingData->UnlockCost);
	LockBuildingData->bIsUnlocked = false;

	TArray<TSoftObjectPtr<UObject>> AssetsToUnload;
	GetSoftObjectPtrsInBuilding(RowName, AssetsToUnload);
	AssetManager->UnloadAsset(AssetsToUnload);
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

		NewBuilding = CreateBuildingInstance(World, RowName);
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

ADDBuildingBase* UDDBuildingManager::CreateBuildingInstance(UWorld* World, const FName& RowName)
{
	IDDFactoryInterface* BuildingFactory = FactoryManager->GetFactory(RowName);
	UObject* CreatedObject = BuildingFactory->CreateObject(World, RowName, FVector::ZeroVector, FRotator::ZeroRotator, nullptr, nullptr);
	return Cast<ADDBuildingBase>(CreatedObject);
}

void UDDBuildingManager::GetSoftObjectPtrsInBuilding(const FName& RowName, TArray<TSoftObjectPtr<UObject>>& AssetsToLoad)
{
	// TODO: 여기에 return 문을 삽입합니다.
	const FDDBuildingBaseData* BuildingData = GetBuildingData(RowName);
	if (!BuildingData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Building Name"));
	}

	// Static Meshes
	for (const TSoftObjectPtr<UStaticMesh>& StaticMesh : BuildingData->StaticMeshs)
	{
		AssetsToLoad.Add(StaticMesh);
	}

	// Skeletal Meshes
	for (const TSoftObjectPtr<USkeletalMesh>& SkeletalMesh : BuildingData->SkeletalMeshs)
	{
		AssetsToLoad.Add(SkeletalMesh);
	}

	// Anim Blueprints
	for (const TSoftObjectPtr<UAnimBlueprint>& AnimBlueprint : BuildingData->AnimBlueprints)
	{
		AssetsToLoad.Add(AnimBlueprint);
	}

	// Anim Montages
	for (const TSoftObjectPtr<UAnimMontage>& Montage : BuildingData->AttackMontages)
	{
		AssetsToLoad.Add(Montage);
	}

	AssetsToLoad.Add(TSoftObjectPtr<UObject>(BuildingData->AttackEffect));

	AssetsToLoad.Add(BuildingData->AttackSound);

	AssetsToLoad.Add(BuildingData->AttackEffect);

	AssetsToLoad.Add(BuildingData->HitEffect);

}

void UDDBuildingManager::HandleBuildingPoolsOnLevelChange()
{
	UWorld* World = GetWorld();
	check(World);

	for (auto& BuildingDataEntry : BuildingDataTable)
	{
		FDDBuildingBaseData* BuildingData = BuildingDataEntry.Value;
		FName RowName = BuildingDataEntry.Key;

		// 빌딩이 잠금 해제되었는지 확인
		if (BuildingData->bIsUnlocked)
		{

			// 풀에 해당 빌딩이 없으면 추가
			if (!BuildingPool.Contains(RowName))
			{
				BuildingPool.Add(RowName, FBuildingList());
			}

			int32 NumInstances = (BuildingData->BuildingType == EBuildingType::Trap) ? 10 : 3;
			for (int32 i = 0; i < NumInstances; ++i)
			{
				ADDBuildingBase* NewBuilding = CreateBuildingInstance(World, RowName);
				if (NewBuilding)
				{
					BuildingPool[RowName].Buildings.Add(NewBuilding);
					NewBuilding->SetActorHiddenInGame(true);  // 일단 숨기기
					NewBuilding->SetActorEnableCollision(false);  // 충돌 비활성화
					NewBuilding->SetActorTickEnabled(false);  // 틱 비활성화
					NewBuilding->SetCanAttack(false);
				}
			}
		}
		else
		{
			// BuildingPool에서 해당 RowName에 해당하는 항목을 찾음
			if (FBuildingList* BuildingList = BuildingPool.Find(RowName))
			{
				// BuildingList 내의 모든 건물 인스턴스를 제거
				for (ADDBuildingBase* Building : BuildingList->Buildings)
				{
					if (Building)
					{
						// 건물 인스턴스 제거 (필요에 따라 Destroy 또는 다른 방식 사용 가능)
						Building->Destroy(); // 또는 적절한 정리 메서드 사용
					}
				}

				// BuildingList 내의 모든 건물 인스턴스를 제거한 후, 배열도 비움
				BuildingList->Buildings.Empty();

				// BuildingPool에서 해당 RowName 제거
				BuildingPool.Remove(RowName);
			}
		}
	}
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
