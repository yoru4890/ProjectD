// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Game/DDDataManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "LSM/Manager/DDBuildingManager.h"
#include "Particles/ParticleSystem.h"

void UDDAssetManager::Initialize()
{

}

void UDDAssetManager::LoadAssetsAsync(const TArray<TSoftObjectPtr<UObject>>& AssetsToLoad)
{
	if (AssetsToLoad.Num() == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No assets to load."));
		return;
	}

	// �񵿱�� �ε��� ��� ����Ʈ�� ����
	TArray<FSoftObjectPath> SoftObjectPaths;
	for (const TSoftObjectPtr<UObject>& Asset : AssetsToLoad)
	{
		if (!Asset.IsValid())
		{
			SoftObjectPaths.Add(Asset.ToSoftObjectPath());
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s is already loaded."), *Asset.ToString());
		}
	}

	// �񵿱� �ε� �Ϸ� �ݹ�
	TFunction<void()> OnAssetsLoadedCallback = [this,AssetsToLoad]()
		{
			for (const TSoftObjectPtr<UObject>& Asset : AssetsToLoad)
			{
				if (Asset.IsValid())
				{
					//UE_LOG(LogTemp, Warning, TEXT("Asset %s loaded successfully."), *Asset.ToString());
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("Start Unload."));
			//this->UnloadAsset(const_cast<TArray<TSoftObjectPtr<UObject>>&>(AssetsToLoad));
		};
	if (SoftObjectPaths.Num() > 0)
	{
		// �񵿱� �ε� ����
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad(SoftObjectPaths, FStreamableDelegate::CreateLambda(OnAssetsLoadedCallback));
	}
}

void UDDAssetManager::UnloadAsset(TArray<TSoftObjectPtr<UObject>>& AssetsToUnload)
{
	UE_LOG(LogTemp, Warning, TEXT("Unload Test"));
	if (AssetsToUnload.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No assets to unload."));
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	// ���� ��ε�
	for (TSoftObjectPtr<UObject>& Asset : AssetsToUnload)
	{
		if (Asset.IsValid())
		{
			// ���� ��θ� ���� ��ε� ����
			StreamableManager.Unload(Asset.ToSoftObjectPath());
			//UE_LOG(LogTemp, Warning, TEXT("Unloading asset: %s"), *Asset.ToString());

			// WeakPtr�� �����Ͽ� ��ε� �� ���� ����
			Asset.ResetWeakPtr();
			//UE_LOG(LogTemp, Warning, TEXT("Asset IsValid after Reset: %s"), Asset.IsValid() ? TEXT("true") : TEXT("false"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Asset %s was already unloaded or invalid."), *Asset.ToString());
		}
	}

	// �ʿ� �� ���� GC ����
	GEngine->ForceGarbageCollection(true);
}

//void UDDAssetManager::RemoveLoadedAssetAll()
//{
//	check(GetWorld());
//	UDDGameInstance* GameInstance = CastChecked<UDDGameInstance>(GetWorld()->GetGameInstance());
//	check(GameInstance);
//
//	UDDBuildingManager* BuildingManager = GameInstance->GetBuildingManager();
//	check(BuildingManager);
//
//	const TMap<FName, FDDBuildingBaseData*>& BuildingDataTable = BuildingManager->GetBuildingDataTable();
//
//	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
//	TArray<FName> KeyArray;
//	BuildingDataTable.GenerateKeyArray(KeyArray);
//	for (FName& RowName : KeyArray)
//	{
//		FDDBuildingBaseData* ObjectStruct = GetObjectBaseData(RowName);
//
//		// �� Ÿ�Ժ��� �ε�� ������ ��ε�
//		for (TSoftObjectPtr<UStaticMesh>& StaticMesh : ObjectStruct->StaticMeshs)
//		{
//			StreamableManager.Unload(StaticMesh.ToSoftObjectPath());
//			StaticMesh.ResetWeakPtr();
//		}
//		for (TSoftObjectPtr<USkeletalMesh>& SkeletalMesh : ObjectStruct->SkeletalMeshs)
//		{
//			StreamableManager.Unload(SkeletalMesh.ToSoftObjectPath());
//			SkeletalMesh.ResetWeakPtr();
//		}
//
//		for (TSoftObjectPtr<UAnimBlueprint>& AnimBlueprint : ObjectStruct->AnimBlueprints)
//		{
//			StreamableManager.Unload(AnimBlueprint.ToSoftObjectPath());
//			AnimBlueprint.ResetWeakPtr();
//		}
//		for (TSoftObjectPtr<UAnimMontage>& Montage : ObjectStruct->AttackMontages)
//		{
//			StreamableManager.Unload(Montage.ToSoftObjectPath());
//			Montage.ResetWeakPtr();
//		}
//
//		StreamableManager.Unload(ObjectStruct->AttackEffect.ToSoftObjectPath());
//		ObjectStruct->AttackEffect.ResetWeakPtr();
//
//		StreamableManager.Unload(ObjectStruct->HitEffect.ToSoftObjectPath());
//		ObjectStruct->HitEffect.ResetWeakPtr();
//
//		StreamableManager.Unload(ObjectStruct->AttackSound.ToSoftObjectPath());
//		ObjectStruct->AttackSound.ResetWeakPtr();
//	}
//}
