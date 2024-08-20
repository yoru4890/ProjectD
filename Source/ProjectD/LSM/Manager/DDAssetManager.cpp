// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/Manager/DDAssetManager.h"
#include "LSM/Building/DDBuildingBaseData.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Game/DDGameSingleton.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "LSM/Manager/DDBuildingManager.h"
#include "Particles/ParticleSystem.h"

void UDDAssetManager::Initialize()
{
	check(GetWorld());
	UDDGameInstance* GameInstance = CastChecked<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(GameInstance);

	UDDBuildingManager* BuildingManager = GameInstance->GetBuildingManager();
	check(BuildingManager);

	const TMap<FName, FDDBuildingBaseData*>& BuildingDataTable = BuildingManager->GetBuildingDataTable();
	UE_LOG(LogTemp, Warning, TEXT("Initialize AssetManager"));
	for (auto& Elem : BuildingDataTable) {
		UE_LOG(LogTemp, Warning, TEXT("%s is Unlock? : %s"), *Elem.Key.ToString(), Elem.Value->bIsUnlocked ? TEXT("true") : TEXT("false"));
		if (Elem.Value->bIsUnlocked) {
			LoadAssetsAsync(Elem.Key);
			UE_LOG(LogTemp, Warning, TEXT(" %s : Init"), *Elem.Key.ToString());
		}
	}
}

void UDDAssetManager::LoadAssetsAsync(const FName& RowName)
{
	FDDBuildingBaseData* ObjectStruct = GetObjectBaseData(RowName);
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

	UE_LOG(LogTemp, Warning, TEXT("%s : LoadAssetsAsync is Started."), *RowName.ToString());
	ObjectStruct->bIsLoading = true;

	// Asset의 경로를 저장하고 있는 Array
	TArray<FSoftObjectPath> SoftObjectPaths;

	// MeshType의 종류에 따라서
	for (const TSoftObjectPtr<UStaticMesh>& StaticMesh : ObjectStruct->StaticMeshs)
	{
		SoftObjectPaths.Add(StaticMesh.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT(" %s : StaticMesh Added"), *RowName.ToString());
	}
	for (const TSoftObjectPtr<USkeletalMesh>& SkeletalMesh : ObjectStruct->SkeletalMeshs)
	{
		if (SkeletalMesh.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : SkeletalMesh isAlready Loaded"), *RowName.ToString());
		}
		SoftObjectPaths.Add(SkeletalMesh.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT(" %s : USkeletalMesh Added"), *RowName.ToString());
	}
	for (const TSoftObjectPtr<UAnimBlueprint>& AnimBlueprint : ObjectStruct->AnimBlueprints)
	{
		if (AnimBlueprint.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : AnimBlueprint isAlready Loaded"), *RowName.ToString());
		}
		SoftObjectPaths.Add(AnimBlueprint.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT(" %s : AnimBlueprint Added"), *RowName.ToString());
	}

	for (const TSoftObjectPtr<UAnimMontage>& Montage : ObjectStruct->AttackMontages)
	{
		if (Montage.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s : Montage isAlready Loaded"), *RowName.ToString());
		}
		SoftObjectPaths.Add(Montage.ToSoftObjectPath());
	}
	if (ObjectStruct->AttackEffect.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT(" %s : Attack Effects isAlready Loaded"), *RowName.ToString());
	}
	SoftObjectPaths.Add(ObjectStruct->AttackEffect.ToSoftObjectPath());

	if (ObjectStruct->HitEffect.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT(" %s :  Effects isAlready Loaded"), *RowName.ToString());
	}
	SoftObjectPaths.Add(ObjectStruct->HitEffect.ToSoftObjectPath());
	// 파티클도 추가
	//for (const TSoftObjectPtr<UParticleSystem>& Effect : ObjectStruct->Effects)
	//{
	//	if (Effect.IsValid())
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT(" %s : Effect isAlready Loaded"), *RowName.ToString());
	//	}
	//	SoftObjectPaths.Add(Effect.ToSoftObjectPath());
	//}

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

FDDBuildingBaseData* UDDAssetManager::GetLoadedAssetByName(const FName& RowName)
{
	UE_LOG(LogTemp, Warning, TEXT("GetLoadedAssetByName is Called"));
	FDDBuildingBaseData* ObjectStruct = GetObjectBaseData(RowName);
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
	FDDBuildingBaseData* ObjectStruct = GetObjectBaseData(RowName);
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

	for (TSoftObjectPtr<UAnimBlueprint>& AnimBlueprint : ObjectStruct->AnimBlueprints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading AnimBlueprint: %s"), *AnimBlueprint.ToString());
		StreamableManager.Unload(AnimBlueprint.ToSoftObjectPath());
		AnimBlueprint.ResetWeakPtr();
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh IsValid after Reset: %s"), AnimBlueprint.IsNull() ? TEXT("false") : TEXT("true"));
	}

	// AnimMontage 언로드
	for (TSoftObjectPtr<UAnimMontage>& Montage : ObjectStruct->AttackMontages)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading AnimMontage: %s"), *Montage.ToString());
		StreamableManager.Unload(Montage.ToSoftObjectPath());
		UE_LOG(LogTemp, Warning, TEXT("Montage Path before Reset: %s"), *Montage.ToSoftObjectPath().ToString());
		Montage.ResetWeakPtr();
		UE_LOG(LogTemp, Warning, TEXT("AnimMontage IsValid after Reset: %s"), Montage.IsValid() ? TEXT("true") : TEXT("false"));
		UE_LOG(LogTemp, Warning, TEXT("Montage Path: %s"), *Montage.ToSoftObjectPath().ToString());
	}

	UE_LOG(LogTemp, Warning, TEXT("Unloading Attack Effect: %s"), *ObjectStruct->AttackEffect.ToString());
	StreamableManager.Unload(ObjectStruct->AttackEffect.ToSoftObjectPath());
	ObjectStruct->AttackEffect.ResetWeakPtr();
	UE_LOG(LogTemp, Warning, TEXT("Attack Effect IsValid after Reset: %s"), ObjectStruct->AttackEffect.IsValid() ? TEXT("true") : TEXT("false"));

	UE_LOG(LogTemp, Warning, TEXT("Unloading Hit Effect: %s"), *ObjectStruct->HitEffect.ToString());
	StreamableManager.Unload(ObjectStruct->HitEffect.ToSoftObjectPath());
	ObjectStruct->HitEffect.ResetWeakPtr();
	UE_LOG(LogTemp, Warning, TEXT("HitEffect IsValid after Reset: %s"), ObjectStruct->HitEffect.IsValid() ? TEXT("true") : TEXT("false"));
	// ParticleSystem 언로드
	//for (TSoftObjectPtr<UParticleSystem>& Effect : ObjectStruct->Effects)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Unloading Effect: %s"), *Effect.ToString());
	//	StreamableManager.Unload(Effect.ToSoftObjectPath());
	//	Effect.ResetWeakPtr();
	//	UE_LOG(LogTemp, Warning, TEXT("Effect IsValid after Reset: %s"), Effect.IsValid() ? TEXT("true") : TEXT("false"));
	//}

	ObjectStruct->bIsLoaded = false;

	// 강제 GC 실행
	GEngine->ForceGarbageCollection(true);
}

void UDDAssetManager::RemoveLoadedAssetAll()
{
	check(GetWorld());
	UDDGameInstance* GameInstance = CastChecked<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(GameInstance);

	UDDBuildingManager* BuildingManager = GameInstance->GetBuildingManager();
	check(BuildingManager);

	const TMap<FName, FDDBuildingBaseData*>& BuildingDataTable = BuildingManager->GetBuildingDataTable();

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TArray<FName> KeyArray;
	BuildingDataTable.GenerateKeyArray(KeyArray);
	for (FName& RowName : KeyArray)
	{
		FDDBuildingBaseData* ObjectStruct = GetObjectBaseData(RowName);

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

		for (TSoftObjectPtr<UAnimBlueprint>& AnimBlueprint : ObjectStruct->AnimBlueprints)
		{
			StreamableManager.Unload(AnimBlueprint.ToSoftObjectPath());
			AnimBlueprint.ResetWeakPtr();
		}
		for (TSoftObjectPtr<UAnimMontage>& Montage : ObjectStruct->AttackMontages)
		{
			StreamableManager.Unload(Montage.ToSoftObjectPath());
			Montage.ResetWeakPtr();
		}

		StreamableManager.Unload(ObjectStruct->AttackEffect.ToSoftObjectPath());
		ObjectStruct->AttackEffect.ResetWeakPtr();

		StreamableManager.Unload(ObjectStruct->HitEffect.ToSoftObjectPath());
		ObjectStruct->HitEffect.ResetWeakPtr();
	}
}

FDDBuildingBaseData* UDDAssetManager::GetObjectBaseData(const FName& RowName)
{
	check(GetWorld());
	UDDGameInstance* GameInstance = CastChecked<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(GameInstance);

	UDDBuildingManager* BuildingManager = GameInstance->GetBuildingManager();
	check(BuildingManager);

	TMap<FName, FDDBuildingBaseData*>& BuildingDataTable = BuildingManager->GetBuildingDataTable();

	FDDBuildingBaseData* ObjectStruct;
	if (BuildingDataTable.Find(RowName)) {
		ObjectStruct = *BuildingDataTable.Find(RowName);
		return ObjectStruct;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%s : RowName is Wrong."), *RowName.ToString());
		return nullptr;
	}
}

