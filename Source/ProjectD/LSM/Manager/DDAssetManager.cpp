// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDAssetManager.h"
#include "LSM/BaseStruct.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Game/DDGameSingleton.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "LSM/DDLoadedAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

void UDDAssetManager::Initialize()
{
	TMap<FName, FDDTrapStruct>& TrapDataTable = UDDGameSingleton::Get().GetTrapDataTable();
	UE_LOG(LogTemp, Warning, TEXT("Initialize AssetManager"));
	for (auto& Elem : TrapDataTable) {
		UE_LOG(LogTemp, Warning, TEXT("%s is Unlock? : %s"), *Elem.Key.ToString(), Elem.Value.bIsTrapUnlocked ? TEXT("true") : TEXT("false"));
		if (Elem.Value.bIsTrapUnlocked) {
			LoadAssetsAsync(Elem.Key);
			UE_LOG(LogTemp, Warning, TEXT(" %s : Init"), *Elem.Key.ToString());
		}
	}
}

void UDDAssetManager::LoadAssetsAsync(const FName& RowName)
{
	FBaseStruct* ObjectStruct = GetObjectBaseData(RowName);
	if (!ObjectStruct)
	{
		return;
	}

	if (ObjectStruct->bIsLoaded)
	{
		// 로딩된 에셋이라면
		UE_LOG(LogTemp, Warning, TEXT("%s : Assets is already loaded."), *RowName.ToString());
		return;
	}

	if (ObjectStruct->bIsLoading)
	{
		// 로딩중인 에셋이라면
		UE_LOG(LogTemp, Warning, TEXT("%s : Assets is Loading."), *RowName.ToString());
		return;
	}

	ObjectStruct->bIsLoading = true;

	// Asset의 경로를 저장하고 있는 Array
	TArray<FSoftObjectPath> SoftObjectPaths;

	// MeshType의 종류에 따라서
	if (ObjectStruct->MeshType == EMeshType::StaticMesh) // StaticMesh라면
	{
		// 경로에 추가
		for (const TSoftObjectPtr<UStaticMesh>& StaticMesh : ObjectStruct->StaticMeshs)
		{
			SoftObjectPaths.Add(StaticMesh.ToSoftObjectPath());
			UE_LOG(LogTemp, Warning, TEXT(" %s : StaticMesh Added"), *RowName.ToString());
		}
	}
	else if (ObjectStruct->MeshType == EMeshType::SkeletalMesh) // SkeletalMesh라면
	{
		// SkletalMesh와 AnimBluePrint를 추가
		for (const TSoftObjectPtr<USkeletalMesh>& SkeletalMesh : ObjectStruct->SkeletalMeshs)
		{
			if (SkeletalMesh.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT(" %s : SkeletalMesh isAlready Loaded"), *RowName.ToString());
			}
			SoftObjectPaths.Add(SkeletalMesh.ToSoftObjectPath());
			UE_LOG(LogTemp, Warning, TEXT(" %s : USkeletalMesh Added"), *RowName.ToString());
		}
		if (ObjectStruct->MyAnimBlueprint.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : AnimBlueprint isAlready Loaded"), *RowName.ToString());
		}
		SoftObjectPaths.Add(ObjectStruct->MyAnimBlueprint.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT(" %s : AnimBlueprint Added"), *RowName.ToString());

		for (const TSoftObjectPtr<UAnimMontage>& Montage : ObjectStruct->AnimMontages)
		{
			if (Montage.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT(" %s : Montage isAlready Loaded"), *RowName.ToString());
			}
			SoftObjectPaths.Add(Montage.ToSoftObjectPath());
			UE_LOG(LogTemp, Warning, TEXT(" %s : MyAnimMontage Added"), *RowName.ToString());
		}
	}
	// 파티클도 추가
	for (const TSoftObjectPtr<UParticleSystem>& Effect : ObjectStruct->Effects)
	{
		if (Effect.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : Effect isAlready Loaded"), *RowName.ToString());
		}
		SoftObjectPaths.Add(Effect.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT(" %s : Effect Added"), *RowName.ToString());
	}

	// Object에 들어있는 Asset의 비동기 로드가 안료되면 호출되는 콜백 함수
	TFunction<void()> OnAssetsLoadedCallback = [this, RowName, ObjectStruct, SoftObjectPaths]()
		{
			ObjectStruct->bIsLoaded = true;
			ObjectStruct->bIsLoading = false;
			UE_LOG(LogTemp, Warning, TEXT(" %s : AsyncLoad is Finished"), *RowName.ToString());

			//if (RowName.IsEqual(FName("UpgradeThornTrap"))) {
			//	RemoveLoadedAssetByName(RowName);
			//}
		};
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	// AssetLoad시작
	StreamableManager.RequestAsyncLoad(SoftObjectPaths, FStreamableDelegate::CreateLambda(OnAssetsLoadedCallback));
}

FBaseStruct* UDDAssetManager::GetLoadedAssetByName(const FName& RowName)
{
	UE_LOG(LogTemp, Warning, TEXT("GetLoadedAssetByName is Called"));
	FBaseStruct* ObjectStruct = GetObjectBaseData(RowName);
	if (!ObjectStruct)
	{
		return nullptr;
	}

	if (ObjectStruct->bIsLoading)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset is Loading"));
		return nullptr;
	}
	return ObjectStruct;
}

void UDDAssetManager::RemoveLoadedAssetByName(const FName& RowName)
{
	FBaseStruct* ObjectStruct = GetObjectBaseData(RowName);
	if (!ObjectStruct)
	{
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	// StaticMesh 언로드
	for (TSoftObjectPtr<UStaticMesh>& StaticMesh : ObjectStruct->StaticMeshs)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading StaticMesh: %s"), *StaticMesh.ToString());
		StreamableManager.Unload(StaticMesh.ToSoftObjectPath());
		StaticMesh.ResetWeakPtr();
		UE_LOG(LogTemp, Warning, TEXT("StaticMesh IsValid after Reset: %s"), StaticMesh.IsValid() ? TEXT("true") : TEXT("false"));
	}

	// SkeletalMesh 언로드
	for (TSoftObjectPtr<USkeletalMesh>& SkeletalMesh : ObjectStruct->SkeletalMeshs)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading SkeletalMesh: %s"), *SkeletalMesh.ToString());
		StreamableManager.Unload(SkeletalMesh.ToSoftObjectPath());
		SkeletalMesh.ResetWeakPtr();
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh IsValid after Reset: %s"), SkeletalMesh.IsNull() ? TEXT("false") : TEXT("true"));
	}

	// AnimBlueprint 언로드
	UE_LOG(LogTemp, Warning, TEXT("Unloading AnimBlueprint: %s"), *ObjectStruct->MyAnimBlueprint.ToString());
	StreamableManager.Unload(ObjectStruct->MyAnimBlueprint.ToSoftObjectPath());
	ObjectStruct->MyAnimBlueprint.ResetWeakPtr();
	UE_LOG(LogTemp, Warning, TEXT("AnimBlueprint IsValid after Reset: %s"), ObjectStruct->MyAnimBlueprint.IsValid() ? TEXT("true") : TEXT("false"));

	// AnimMontage 언로드
	for (TSoftObjectPtr<UAnimMontage>& Montage : ObjectStruct->AnimMontages)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading AnimMontage: %s"), *Montage.ToString());
		StreamableManager.Unload(Montage.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT("Montage Path before Reset: %s"), *Montage.ToSoftObjectPath().ToString());
		Montage.ResetWeakPtr();
		UE_LOG(LogTemp, Warning, TEXT("AnimMontage IsValid after Reset: %s"), Montage.IsValid() ? TEXT("true") : TEXT("false"));
		UE_LOG(LogTemp, Warning, TEXT("Montage Path: %s"), *Montage.ToSoftObjectPath().ToString());
	}

	// ParticleSystem 언로드
	for (TSoftObjectPtr<UParticleSystem>& Effect : ObjectStruct->Effects)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading Effect: %s"), *Effect.ToString());
		StreamableManager.Unload(Effect.ToSoftObjectPath());
		Effect.ResetWeakPtr();
		UE_LOG(LogTemp, Warning, TEXT("Effect IsValid after Reset: %s"), Effect.IsValid() ? TEXT("true") : TEXT("false"));
	}

	ObjectStruct->bIsLoaded = false;

	// 강제 GC 실행
	GEngine->ForceGarbageCollection(true);
}

void UDDAssetManager::RemoveLoadedAssetAll()
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TMap<FName, FDDTrapStruct>& TrapDataTable = UDDGameSingleton::Get().GetTrapDataTable();
	TArray<FName> KeyArray;
	TrapDataTable.GenerateKeyArray(KeyArray);
	for (FName& RowName : KeyArray)
	{
		FBaseStruct* ObjectStruct = GetObjectBaseData(RowName);

		// 각 타입별로 로드된 에셋을 언로드
		for (TSoftObjectPtr<UStaticMesh>& StaticMesh : ObjectStruct->StaticMeshs)
		{
			StreamableManager.Unload(StaticMesh.ToSoftObjectPath());
			StaticMesh.ResetWeakPtr();
		}
		for (TSoftObjectPtr<USkeletalMesh>& SkeletalMesh : ObjectStruct->SkeletalMeshs)
		{
			StreamableManager.Unload(SkeletalMesh.ToSoftObjectPath());
			SkeletalMesh.ResetWeakPtr();
		}
		StreamableManager.Unload(ObjectStruct->MyAnimBlueprint.ToSoftObjectPath());
		ObjectStruct->MyAnimBlueprint.ResetWeakPtr();
		for (TSoftObjectPtr<UAnimMontage>& Montage : ObjectStruct->AnimMontages)
		{
			StreamableManager.Unload(Montage.ToSoftObjectPath());
			Montage.ResetWeakPtr();
		}
		for (TSoftObjectPtr<UParticleSystem>& Effect : ObjectStruct->Effects)
		{
			StreamableManager.Unload(Effect.ToSoftObjectPath());
			Effect.ResetWeakPtr();
		}
	}
}

FBaseStruct* UDDAssetManager::GetObjectBaseData(const FName& RowName)
{
	FBaseStruct* ObjectStruct;
	if (UDDGameSingleton::Get().GetTrapDataTable().Find(RowName)) {
		ObjectStruct = UDDGameSingleton::Get().GetTrapDataTable().Find(RowName);
		return ObjectStruct;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%s : RowName is Wrong."), *RowName.ToString());
		return nullptr;
	}
}

