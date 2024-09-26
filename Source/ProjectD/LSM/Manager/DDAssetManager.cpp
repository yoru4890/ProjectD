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

void UDDAssetManager::LoadAssetsAsync(const TArray<TSoftObjectPtr<UObject>>& AssetsToLoad, FStreamableDelegate OnAssetsLoadedCallback)
{
	if (AssetsToLoad.Num() == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No assets to load."));
		OnAssetsLoadedCallback.ExecuteIfBound();
		return;
	}

	// �񵿱�� �ε��� ��� ����Ʈ�� ����
	TArray<FSoftObjectPath> SoftObjectPaths;
	for (const TSoftObjectPtr<UObject>& Asset : AssetsToLoad)
	{
		// TODO : �̰� ���߿� Ȯ���ؾ���!!!! 
		// ������ �󿡼��� IsValid�� üũ���� ��, �ε��� �Ǿ��ٰ� ������ ���� �Ẹ�� �ε��� �ȵǾ��־ �ƹ��͵� �ȶ�.
		// �׷��� ù��°�Ÿ� ����� ������ 2���� ������ �� �߶��
		// 2��°�Ŵ� �����ͻ󿡼� 2���� ���ӽ����ص� ������ ���.
		//if (!Asset.IsNull())
		//if (!Asset.IsNull() && !Asset.IsValid())
		if (!Asset.IsNull())
		{
			FSoftObjectPath Path = Asset.ToSoftObjectPath();
			SoftObjectPaths.Add(Path);
			UE_LOG(LogTemp, Log, TEXT("Asset to load: %s"), *Path.ToString());  // ��� �α� �߰�
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Asset is null: %s"), *Asset.ToString());  // �� ������ üũ
		}
	}
	if (SoftObjectPaths.Num() > 0)
	{
		// �񵿱� �ε� ����
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad(SoftObjectPaths, OnAssetsLoadedCallback);
	}
	else
	{
		OnAssetsLoadedCallback.ExecuteIfBound();
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

bool UDDAssetManager::ShouldForceGarbageCollection() const
{
	UE_LOG(LogTemp, Warning, TEXT("Garbage Collection Start"));
	const int32 MemoryThresholdMB = 500; // 500MB ����
	int32 CurrentMemoryUsageMB = GetCurrentMemoryUsageInMB();

	return CurrentMemoryUsageMB > MemoryThresholdMB;
}

int32 UDDAssetManager::GetCurrentMemoryUsageInMB() const
{
	return FPlatformMemory::GetStats().UsedPhysical / (1024 * 1024);
}
