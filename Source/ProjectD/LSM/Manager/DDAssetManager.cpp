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
	// �̹� �ε��� Object�� return
	if (LoadedAssets.Find(RowName)) {
		return;
	}

	// Asset�� ��θ� �����ϰ� �ִ� Array
	TArray<FSoftObjectPath> SoftObjectPaths;

	// MeshType�� ������ ����
	if (ObjectStruct.MeshType == EMeshType::StaticMesh) // StaticMesh���
	{
		// ��ο� �߰�
		for (const TSoftObjectPtr<UStaticMesh>& StaticMesh : ObjectStruct.StaticMeshs)
		{
			SoftObjectPaths.Add(StaticMesh.ToSoftObjectPath());
			UE_LOG(LogTemp, Warning, TEXT(" %s : StaticMesh Added"), *RowName.ToString());
		}
	}
	else if (ObjectStruct.MeshType == EMeshType::SkeletalMesh) // SkeletalMesh���
	{
		// SkletalMesh�� AnimBluePrint�� �߰�
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
	// ��ƼŬ�� �߰�
	for (const TSoftObjectPtr<UParticleSystem>& Effect : ObjectStruct.Effects)
	{
		SoftObjectPaths.Add(Effect.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT(" %s : Effect Added"), *RowName.ToString());
	}

	// Object�� ����ִ� Asset�� �񵿱� �ε尡 �ȷ�Ǹ� ȣ��Ǵ� �ݹ� �Լ�
	TFunction<void()> OnAssetsLoadedCallback = [this, RowName, ObjectStruct, SoftObjectPaths]()
		{
			// ���µ��� ������ Struct
			FLoadedAsset LoadedAsset;

			// ��ε��� �ҷ��ͼ�
			for (const FSoftObjectPath& SoftObjectPath : SoftObjectPaths)
			{
				// �ε�� Asset���� �ҷ��� ��
				UObject* LoadedObject = SoftObjectPath.ResolveObject();
				UE_LOG(LogTemp, Warning, TEXT(" %s : Asset Loaded -> %s"), *RowName.ToString(), *SoftObjectPath.ToString());
				// �´� Class�� �з��ѵ� �־��ش�
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
	// AssetLoad����
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

