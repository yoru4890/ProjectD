// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDAssetManager.h"
#include "LSM/BaseStruct.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Game/DDGameSingleton.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

void UDDAssetManager::Initialize()
{
	TMap<FName, FDDTrapStruct>& TrapDataTable = UDDGameSingleton::Get().GetTrapDataTable();
	UE_LOG(LogTemp, Warning, TEXT("Initialize AssetManager"));
	for (auto& Elem : TrapDataTable) {
		UE_LOG(LogTemp, Warning, TEXT("%s is Unlock? : %s"), *Elem.Key.ToString(), Elem.Value.bIsTrapUnlocked ? TEXT("true") : TEXT("false"));
		if (Elem.Value.bIsTrapUnlocked) {
			LoadAssetsAsync(Elem.Key, Elem.Value);
			UE_LOG(LogTemp, Warning, TEXT(" %s : Init"), *Elem.Key.ToString());
		}
	}
}

void UDDAssetManager::LoadAssetsAsync(const FName& RowName, const FBaseStruct& ObjectStruct)
{
	// 이미 로드한 Object면 return
	if (LoadedAssets.Find(RowName)) {
		return;
	}

	// Asset의 경로를 저장하고 있는 Array
	TArray<FSoftObjectPath> SoftObjectPaths;

	// MeshType의 종류에 따라서
	if (ObjectStruct.MeshType == EMeshType::StaticMesh) // StaticMesh라면
	{
		// 경로에 추가
		for (const TSoftObjectPtr<UStaticMesh>& StaticMesh : ObjectStruct.StaticMeshs)
		{
			SoftObjectPaths.Add(StaticMesh.ToSoftObjectPath());
			UE_LOG(LogTemp, Warning, TEXT(" %s : StaticMesh Added"), *RowName.ToString());
		}
	}
	else if (ObjectStruct.MeshType == EMeshType::SkeletalMesh) // SkeletalMesh라면
	{
		// SkletalMesh와 AnimBluePrint를 추가
		for (const TSoftObjectPtr<USkeletalMesh>& SkeletalMesh : ObjectStruct.SkeletalMeshs)
		{
			SoftObjectPaths.Add(SkeletalMesh.ToSoftObjectPath());
			UE_LOG(LogTemp, Warning, TEXT(" %s : USkeletalMesh Added"), *RowName.ToString());
		}
		if (ObjectStruct.AnimBlueprint.IsValid())
		{
			SoftObjectPaths.Add(ObjectStruct.AnimBlueprint.ToSoftObjectPath());
			UE_LOG(LogTemp, Warning, TEXT(" %s : AnimBlueprint Added"), *RowName.ToString());
		}
	}
	// 파티클도 추가
	for (const TSoftObjectPtr<UParticleSystem>& Effect : ObjectStruct.Effects)
	{
		SoftObjectPaths.Add(Effect.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT(" %s : Effect Added"), *RowName.ToString());
	}

	// Object에 들어있는 Asset의 비동기 로드가 안료되면 호출되는 콜백 함수
	TFunction<void()> OnAssetsLoadedCallback = [this, RowName, ObjectStruct, SoftObjectPaths]()
		{
			// 에셋들을 저장할 Struct
			FLoadedAsset LoadedAsset;

			// 경로들을 불러와서
			for (const FSoftObjectPath& SoftObjectPath : SoftObjectPaths)
			{
				// 로드된 Asset들을 불러온 뒤
				UObject* LoadedObject = SoftObjectPath.ResolveObject();
				UE_LOG(LogTemp, Warning, TEXT(" %s : Asset Loaded -> %s"), *RowName.ToString(), *SoftObjectPath.ToString());
				// 맞는 Class로 분류한뒤 넣어준다
				if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(LoadedObject))
				{
					LoadedAsset.StaticMeshs.Add(StaticMesh);
				}
				else if (USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(LoadedObject))
				{
					LoadedAsset.SkeletalMeshs.Add(SkeletalMesh);
				}
				else if (UAnimBlueprint* AnimBP = Cast<UAnimBlueprint>(LoadedObject))
				{
					LoadedAsset.AnimBlueprint = AnimBP;
				}
				else if (UParticleSystem* ParticleSystem = Cast<UParticleSystem>(LoadedObject))
				{
					LoadedAsset.Effects.Add(ParticleSystem);
				}
			}
			OnAssetsLoaded(RowName, LoadedAsset);
		};
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	// AssetLoad시작
	StreamableManager.RequestAsyncLoad(SoftObjectPaths, FStreamableDelegate::CreateLambda(OnAssetsLoadedCallback));
}

FLoadedAsset& UDDAssetManager::GetLoadedAssetByName(const FName& RowName)
{
	return *LoadedAssets.Find(RowName);
}

void UDDAssetManager::RemoveLoadedAssetByName(const FName& RowName)
{
	LoadedAssets.Remove(RowName);
}

void UDDAssetManager::RevoeLoadedAssetAll()
{
	LoadedAssets.Empty();
}

void UDDAssetManager::OnAssetsLoaded(const FName& RowName, FLoadedAsset& LoadedAsset)
{
	LoadedAssets.Add(RowName, LoadedAsset);
}

