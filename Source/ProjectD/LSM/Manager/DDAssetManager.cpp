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

	// 비동기로 로드할 경로 리스트를 생성
	TArray<FSoftObjectPath> SoftObjectPaths;
	for (const TSoftObjectPtr<UObject>& Asset : AssetsToLoad)
	{
		// TODO : 이거 나중에 확인해야함!!!! 
		// 에디터 상에서는 IsValid로 체크했을 떄, 로딩이 되었다고 뜨지만 막상 써보면 로딩이 안되어있어서 아무것도 안뜸.
		// 그래서 첫번째거를 해줘야 게임을 2번쨰 실행할 때 잘뜬다
		// 2번째거는 에디터상에서 2번쨰 게임실행해도 오류가 뜬다.
		//if (!Asset.IsNull())
		//if (!Asset.IsNull() && !Asset.IsValid())
		if (!Asset.IsNull())
		{
			FSoftObjectPath Path = Asset.ToSoftObjectPath();
			SoftObjectPaths.Add(Path);
			UE_LOG(LogTemp, Log, TEXT("Asset to load: %s"), *Path.ToString());  // 경로 로그 추가
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Asset is null: %s"), *Asset.ToString());  // 널 포인터 체크
		}
	}
	if (SoftObjectPaths.Num() > 0)
	{
		// 비동기 로드 시작
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

	// 에셋 언로드
	for (TSoftObjectPtr<UObject>& Asset : AssetsToUnload)
	{
		if (Asset.IsValid())
		{
			// 에셋 경로를 통해 언로드 진행
			StreamableManager.Unload(Asset.ToSoftObjectPath());
			//UE_LOG(LogTemp, Warning, TEXT("Unloading asset: %s"), *Asset.ToString());

			// WeakPtr를 리셋하여 언로드 후 참조 제거
			Asset.ResetWeakPtr();
			//UE_LOG(LogTemp, Warning, TEXT("Asset IsValid after Reset: %s"), Asset.IsValid() ? TEXT("true") : TEXT("false"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Asset %s was already unloaded or invalid."), *Asset.ToString());
		}
	}

	// 필요 시 강제 GC 실행
	GEngine->ForceGarbageCollection(true);
}

bool UDDAssetManager::ShouldForceGarbageCollection() const
{
	UE_LOG(LogTemp, Warning, TEXT("Garbage Collection Start"));
	const int32 MemoryThresholdMB = 500; // 500MB 기준
	int32 CurrentMemoryUsageMB = GetCurrentMemoryUsageInMB();

	return CurrentMemoryUsageMB > MemoryThresholdMB;
}

int32 UDDAssetManager::GetCurrentMemoryUsageInMB() const
{
	return FPlatformMemory::GetStats().UsedPhysical / (1024 * 1024);
}
