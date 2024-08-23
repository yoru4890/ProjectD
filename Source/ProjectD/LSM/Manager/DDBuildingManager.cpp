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
			// ���� �Ŵ������� �ε� ��û
			GetSoftObjectPtrsInBuilding(Elem.Key, AssetsToLoad);
			if (AssetManager)
			{
				AssetManager->LoadAssetsAsync(AssetsToLoad); // ���� ����Ʈ ����
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

	// Ǯ���� Ʈ���� �����ϴ�.
	if (BuildingPool[RowName].Buildings.Num() > 0) {
		NewBuilding = BuildingPool[RowName].Buildings.Pop();
		if (NewBuilding) {
			// Ʈ�� ��ġ�� ȸ���� �����մϴ�.
			NewBuilding->SetActorLocationAndRotation(Location, Rotation);
		}
	}
	else {
		// World�� null�̸� ���� ����
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
	// TODO: ���⿡ return ���� �����մϴ�.
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

		// ������ ��� �����Ǿ����� Ȯ��
		if (BuildingData->bIsUnlocked)
		{

			// Ǯ�� �ش� ������ ������ �߰�
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
					NewBuilding->SetActorHiddenInGame(true);  // �ϴ� �����
					NewBuilding->SetActorEnableCollision(false);  // �浹 ��Ȱ��ȭ
					NewBuilding->SetActorTickEnabled(false);  // ƽ ��Ȱ��ȭ
					NewBuilding->SetCanAttack(false);
				}
			}
		}
		else
		{
			// BuildingPool���� �ش� RowName�� �ش��ϴ� �׸��� ã��
			if (FBuildingList* BuildingList = BuildingPool.Find(RowName))
			{
				// BuildingList ���� ��� �ǹ� �ν��Ͻ��� ����
				for (ADDBuildingBase* Building : BuildingList->Buildings)
				{
					if (Building)
					{
						// �ǹ� �ν��Ͻ� ���� (�ʿ信 ���� Destroy �Ǵ� �ٸ� ��� ��� ����)
						Building->Destroy(); // �Ǵ� ������ ���� �޼��� ���
					}
				}

				// BuildingList ���� ��� �ǹ� �ν��Ͻ��� ������ ��, �迭�� ���
				BuildingList->Buildings.Empty();

				// BuildingPool���� �ش� RowName ����
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
