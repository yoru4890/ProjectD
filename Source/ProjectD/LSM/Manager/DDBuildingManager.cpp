// Fill out your copyright notice in the Description page of Project Settings.

#include "LSM/Manager/DDBuildingManager.h"
#include "YSY/Game/DDDataManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "DDFactoryManager.h"
#include "DDAssetManager.h"
#include "LSM/Factory/DDFactoryInterface.h"
#include "GameFramework/Actor.h"
#include "YSY/Game/DDPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Building/DDBuildingBase.h"

#pragma region Initalize

UDDBuildingManager::UDDBuildingManager()
{

}

void UDDBuildingManager::Initialize()
{
	SetupCommonReferences(GetWorld());

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
	//HandleBuildingPoolsOnLevelChange();
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
			FDDBuildingBaseData* ParentBuildingData = (*BuildingDataTable.Find(ParentName));
			TotalCost += ParentBuildingData->BuildCost;
			ParentName = (*BuildingDataTable.Find(ParentName))->ParentRowName;
		}

		TotalCost = TotalCost * Ratio;

		BuildingData->SellCost = TotalCost;
	}
}

#pragma region SpawnAndDestroy

ADDBuildingBase* UDDBuildingManager::SpawnBuilding(UWorld* World, const FName& RowName, const FVector& Location, const FRotator& Rotation, AActor* Owner, APawn* Instigator)
{
	if (!BuildingPool.Contains(RowName))
	{
		BuildingPool.Add(RowName, FBuildingList());
	}

	ADDBuildingBase* NewBuilding = nullptr;

	if (BuildingPool[RowName].Buildings.Num() > 0)
	{
		NewBuilding = BuildingPool[RowName].Buildings.Pop();
		if (NewBuilding)
		{

		}
	}
	else
	{
		check(World);
		FDDFactoryParams Params;
		Params.World = World;
		Params.RowName = RowName;
		Params.Owner = Owner;
		Params.Instigator = Instigator;
		NewBuilding = CreateBuildingInstance(Params);
	}
	if (NewBuilding)
	{
		NewBuilding->SetActorLocationAndRotation(Location, Rotation);
		NewBuilding->SetActorHiddenInGame(false);
		NewBuilding->SetActorEnableCollision(true);
		NewBuilding->SetActorTickEnabled(true);
		NewBuilding->SetCanAttack(false);
	}

	return NewBuilding;
}

void UDDBuildingManager::DestroyBuilding(ADDBuildingBase& Building)
{
	BuildingPool[Building.GetRowName()].Buildings.Add(Building);
	Building.SetBuildingToOriginal();
	Building.SetActorHiddenInGame(true);
	Building.SetActorEnableCollision(false);
	Building.SetActorTickEnabled(false);
	Building.SetCanAttack(false);
}

ADDBuildingBase* UDDBuildingManager::CreateBuildingInstance(const FDDFactoryParams& Params)
{
	IDDFactoryInterface* BuildingFactory = FactoryManager->GetFactory(Params.RowName);
	if (!BuildingFactory)
	{
		UE_LOG(LogTemp,Warning,TEXT("Can't Find Factory"))
		return nullptr;
	}
	UObject* CreatedObject = BuildingFactory->CreateObject(Params);
	return Cast<ADDBuildingBase>(CreatedObject);
}

#pragma endregion SpawnAndDestroy

#pragma endregion Initalize

#pragma region Utility

bool UDDBuildingManager::IsBuildingUnlocked(const FName& RowName) const
{
	const FDDBuildingBaseData* BuildingStruct = GetBuildingData(RowName);

	if (!BuildingStruct)
	{
		return false;
	}

	bool bIsUnlocked = BuildingStruct->bIsUnlocked;

	return bIsUnlocked;

}

bool UDDBuildingManager::IsParentUnlocked(const FName& RowName)
{
	FDDBuildingBaseData* BuildingStruct = GetBuildingData(RowName);

	if (!BuildingStruct)
	{
		return false;
	}

	FName ParentName = BuildingStruct->ParentRowName;

	while (ParentName != FName("None"))
	{
		if (!GetBuildingData(ParentName)->bIsUnlocked)
		{
			UE_LOG(LogTemp, Warning, TEXT("Parent Trap is Not Unlocked"));
			return false;
		}
		ParentName = GetBuildingData(ParentName)->ParentRowName;
	}

	return true;
}

bool UDDBuildingManager::UnlockBuilding(const FName& RowName)
{
	FDDBuildingBaseData* BuildingStruct = GetBuildingData(RowName);

	if (!BuildingStruct)
	{
		return false;
	}

	FName ParentName = BuildingStruct->ParentRowName;

	while (ParentName != FName("None"))
	{
		if (!GetBuildingData(ParentName)->bIsUnlocked)
		{
			UE_LOG(LogTemp, Warning, TEXT("Parent Trap is Not Unlocked"));
			return false;
		}
		ParentName = GetBuildingData(ParentName)->ParentRowName;

	}
	UE_LOG(LogTemp, Warning, TEXT("Here2"));
	ADDPlayerState* PlayerState = CastChecked<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	check(PlayerState);

	if (PlayerState->CheckLikePoint(BuildingStruct->UnlockCost))
	{
		PlayerState->SubtractLikePoint(BuildingStruct->UnlockCost);
		UE_LOG(LogTemp, Warning, TEXT("Here3"));
		BuildingStruct->bIsUnlocked = true;

		PlayerState->UpdateBuildingLockState(RowName, true);

		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Here4"));
	return false;
}

bool UDDBuildingManager::LockBuilding(const FName& RowName)
{
	FDDBuildingBaseData* BuildingStruct = GetBuildingData(RowName);

	if (!BuildingStruct)
	{
		return false;
	}

	TArray<FName> Stack;
	Stack.Push(RowName);

	while (Stack.Num() > 0)
	{
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

	PlayerState->UpdateBuildingLockState(RowName, false);

	return true;
}

#pragma endregion Utility

#pragma region Asset

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
	/*for (const TSoftObjectPtr<UAnimBlueprint>& AnimBlueprint : BuildingData->AnimBlueprints)
	{
		AssetsToLoad.Add(AnimBlueprint);
	}*/

	// Anim Montages
	for (const TSoftObjectPtr<UAnimMontage>& Montage : BuildingData->AttackMontages)
	{
		AssetsToLoad.Add(Montage);
	}

	AssetsToLoad.Add(BuildingData->AttackEffect);

	AssetsToLoad.Add(BuildingData->AttackSound);

	AssetsToLoad.Add(BuildingData->AttackEffect);

	AssetsToLoad.Add(BuildingData->HitEffect);

}

#pragma endregion Asset

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
			LoadBuildingAssets(RowName);
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
			UnloadBuildingAssets(RowName);
		}
	}
}

void UDDBuildingManager::AllClearPools()
{
	UWorld* World = GetWorld();
	check(World);

	for (auto& BuildingDataEntry : BuildingDataTable)
	{
		FDDBuildingBaseData* BuildingData = BuildingDataEntry.Value;
		FName RowName = BuildingDataEntry.Key;
		if (FBuildingList* BuildingList = BuildingPool.Find(RowName))
		{
			for (ADDBuildingBase* Building : BuildingList->Buildings)
			{
				if (Building)
				{
					Building->Destroy(); // �Ǵ� ������ ���� �޼��� ���
				}
			}
			BuildingList->Buildings.Empty();
			BuildingPool.Remove(RowName);
		}
		UnloadBuildingAssets(RowName);
	}
}

void UDDBuildingManager::LoadBuildingAssets(const FName& RowName)
{
	TArray<TSoftObjectPtr<UObject>> AssetsToLoad;

	GetSoftObjectPtrsInBuilding(RowName, AssetsToLoad);

	AssetManager->LoadAssetsAsync(AssetsToLoad, FStreamableDelegate::CreateLambda([this, RowName]()
		{
			OnBuildingAssetsLoaded(RowName);
		}));

}

void UDDBuildingManager::UnloadBuildingAssets(const FName& RowName)
{
	TArray<TSoftObjectPtr<UObject>> AssetsToUnLoad;

	GetSoftObjectPtrsInBuilding(RowName, AssetsToUnLoad);

	AssetManager->UnloadAsset(AssetsToUnLoad);
}

void UDDBuildingManager::OnBuildingAssetsLoaded(const FName& RowName)
{
	// �ε��� RowName�� �ش��ϴ� ���͸� Ǯ�� �߰�
	UE_LOG(LogTemp, Warning, TEXT("Assets for RowName %s loaded."), *RowName.ToString());

	// Ǯ�� �ش� RowName�� ���� ���� �߰�
	if (!BuildingPool.Contains(RowName))
	{
		BuildingPool.Add(RowName, FBuildingList());
	}
	FDDBuildingBaseData* BuildingData = GetBuildingData(RowName);

	// ������ ��� �����Ǿ����� Ȯ��
	if (BuildingData->bIsUnlocked)
	{

		// Ǯ�� �ش� ������ ������ �߰�
		if (!BuildingPool.Contains(RowName))
		{
			BuildingPool.Add(RowName, FBuildingList());
		}

		FDDFactoryParams Params;
		Params.World = GetWorld();
		Params.RowName = RowName;
		Params.Owner = nullptr;
		Params.Instigator = nullptr;

		int32 NumInstances = (BuildingData->BuildingType == EBuildingType::Trap) ? 10 : 3;
		for (int32 i = 0; i < NumInstances; ++i)
		{
			ADDBuildingBase* NewBuilding = CreateBuildingInstance(Params);
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
}

void UDDBuildingManager::SaveUnlockBuilding(const TMap<FName, bool>& BuildingUnlocked)
{
	for (const auto& [BuildingName, Unlocked] : BuildingUnlocked)
	{
		FDDBuildingBaseData* BuildingData = GetBuildingData(BuildingName);

		BuildingData->bIsUnlocked = Unlocked;
	}
}

#pragma region Data

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

	const FDDTrapData* FoundData = MyGameInstance->GetDataManager()->GetTrapDataTable().Find(RowName);
	if (!FoundData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trap data for RowName %s not found!"), *RowName.ToString());
	}
	return FoundData;
}

const FDDTowerData* UDDBuildingManager::GetTowerData(const FName& RowName) const
{
	const FDDTowerData* FoundData = MyGameInstance->GetDataManager()->GetTowerDataTable().Find(RowName);
	if (!FoundData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tower data for RowName %s not found!"), *RowName.ToString());
	}
	return FoundData;
}

const TMap<FName, FDDTrapData>& UDDBuildingManager::GetTrapDataTable() const
{
	return MyGameInstance->GetDataManager()->GetTrapDataTable();
}

TMap<FName, FDDTrapData>& UDDBuildingManager::GetTrapDataTable()
{
	return MyGameInstance->GetDataManager()->GetTrapDataTable();
}

const TMap<FName, FDDTowerData>& UDDBuildingManager::GetTowerDataTable() const
{
	return MyGameInstance->GetDataManager()->GetTowerDataTable();
}

TMap<FName, FDDTowerData>& UDDBuildingManager::GetTowerDataTable()
{
	return MyGameInstance->GetDataManager()->GetTowerDataTable();
}

const TMap<FName, FDDBuildingBaseData*>& UDDBuildingManager::GetBuildingDataTable() const
{
	return BuildingDataTable;
}

TMap<FName, FDDBuildingBaseData*>& UDDBuildingManager::GetBuildingDataTable()
{
	return BuildingDataTable;
}


#pragma endregion Data