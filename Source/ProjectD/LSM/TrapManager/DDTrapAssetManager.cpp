// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapManager/DDTrapAssetManager.h"
#include "Engine/StreamableManager.h"
#include "YSY/Game/DDGameSingleton.h"
#include "YSY/Game/DDGameInstance.h"
#include "Particles/ParticleSystem.h"
#include "Engine/AssetManager.h"


UDDTrapAssetManager::UDDTrapAssetManager()
{

}

void UDDTrapAssetManager::LoadTrapAssetsAsync()
{
	TObjectPtr<UDDGameInstance> MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);

	const TMap<FName, FDDTrapStruct>& TrapDataTable = UDDGameSingleton::Get().GetTrapDataTable();

	for (auto& Elem : TrapDataTable)
	{
		FTrapAssetsToLoad TrapAssetsToLoad;
		if (Elem.Value.TrapMeshType == ETrapMeshType::StaticMesh) {
			TrapAssetsToLoad.StaticMesh = Elem.Value.TrapStaticMesh.ToSoftObjectPath();
		}
		else {
			TrapAssetsToLoad.SkeletalMesh = Elem.Value.TrapSkeletalMesh.ToSoftObjectPath();
		}

		TrapAssetsToLoad.Effect = Elem.Value.TrapEffect.ToSoftObjectPath();
		TrapAssetsToLoad.AnimBlueprint = Elem.Value.TrapAnimBlueprint.ToSoftObjectPath();

		if (TrapAssetsToLoad.StaticMesh.IsValid() || TrapAssetsToLoad.SkeletalMesh.IsValid() || TrapAssetsToLoad.Effect.IsValid() || TrapAssetsToLoad.AnimBlueprint.IsValid())
		{
			TrapAssetsToLoads.Add(Elem.Key, TrapAssetsToLoad);
		}
	}

	// 비동기 로드 시작
	TArray<FSoftObjectPath> AssetsToLoad;
	for (const TPair<FName, FTrapAssetsToLoad>& AssetPair : TrapAssetsToLoads)
	{
		if (AssetPair.Value.StaticMesh.IsValid())
		{
			AssetsToLoad.Add(AssetPair.Value.StaticMesh);
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : StaticMesh's Path is Invalid"), *AssetPair.Key.ToString());
		}
		if (AssetPair.Value.SkeletalMesh.IsValid()) 
		{
			AssetsToLoad.Add(AssetPair.Value.SkeletalMesh);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : SkeletalMesh's Path is Invalid"), *AssetPair.Key.ToString());
		}
		if (AssetPair.Value.Effect.IsValid()) 
		{
			AssetsToLoad.Add(AssetPair.Value.Effect);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : Effect's Path is Invalid"), *AssetPair.Key.ToString());
		}
		if (AssetPair.Value.AnimBlueprint.IsValid())
		{
			AssetsToLoad.Add(AssetPair.Value.AnimBlueprint);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : AnimBlueprint's Path is Invalid"), *AssetPair.Key.ToString());
		}
	}

	MyGameInstance->GetStreamableManager().RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &UDDTrapAssetManager::OnTrapAssetsLoaded));
}

void UDDTrapAssetManager::OnTrapAssetsLoaded()
{
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);
	UE_LOG(LogTemp, Log, TEXT("OnTrapAssetsLoaded : TrapAssetsToLoadNum -> %d"), TrapAssetsToLoads.Num());

	for (const TPair<FName, FTrapAssetsToLoad>& AssetPair : TrapAssetsToLoads)
	{
		FLoadedTrapAsset LoadedAssets;
		LoadAssetIfValid<UStaticMesh>(MyGameInstance, AssetPair.Value.StaticMesh, LoadedAssets.StaticMesh);
		LoadAssetIfValid<USkeletalMesh>(MyGameInstance, AssetPair.Value.SkeletalMesh, LoadedAssets.SkeletalMesh);
		LoadAssetIfValid<UParticleSystem>(MyGameInstance, AssetPair.Value.Effect, LoadedAssets.Effect);
		LoadAssetIfValid<UAnimBlueprint>(MyGameInstance, AssetPair.Value.AnimBlueprint, LoadedAssets.AnimBlueprint);

		if (LoadedAssets.StaticMesh || LoadedAssets.SkeletalMesh || LoadedAssets.Effect || LoadedAssets.AnimBlueprint)
		{
			LoadedTrapAssets.Add(AssetPair.Key, LoadedAssets);
		}

	}


	// 로드 완료 처리
	TrapAssetsToLoads.Empty();
}

template<typename AssetType>
void UDDTrapAssetManager::LoadAssetIfValid(UDDGameInstance* MyGameInstance, const FSoftObjectPath& AssetPath, TObjectPtr<AssetType>& OutLoadedAsset)
{
	if (AssetPath.IsValid())
	{
		UObject* LoadedAsset = MyGameInstance->GetStreamableManager().LoadSynchronous(AssetPath);
		if (AssetType* TypedLoadedAsset = Cast<AssetType>(LoadedAsset))
		{
			OutLoadedAsset = TypedLoadedAsset;
			UE_LOG(LogTemp, Log, TEXT("%s loaded: %s"), *AssetType::StaticClass()->GetName(), *AssetPath.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load asset: %s"), *AssetPath.ToString());
		}
	}
}

UStaticMesh* UDDTrapAssetManager::GetStaticMesh(const FName& TrapName)
{
	if (FLoadedTrapAsset* LoadedAssets = LoadedTrapAssets.Find(TrapName))
	{
		return LoadedAssets->StaticMesh;
	}
	return nullptr;
}

USkeletalMesh* UDDTrapAssetManager::GetSkeletalMesh(const FName& TrapName)
{
	if (FLoadedTrapAsset* LoadedAssets = LoadedTrapAssets.Find(TrapName))
	{
		return LoadedAssets->SkeletalMesh;
	}
	return nullptr;
}

UAnimBlueprint* UDDTrapAssetManager::GetAnimBlueprint(const FName& TrapName)
{
	if (FLoadedTrapAsset* LoadedAssets = LoadedTrapAssets.Find(TrapName))
	{
		return LoadedAssets->AnimBlueprint;
	}
	return nullptr;
}

UParticleSystem* UDDTrapAssetManager::GetParticleEffect(const FName& TrapName)
{
	if (FLoadedTrapAsset* LoadedAssets = LoadedTrapAssets.Find(TrapName))
	{
		return LoadedAssets->Effect;
	}
	return nullptr;
}