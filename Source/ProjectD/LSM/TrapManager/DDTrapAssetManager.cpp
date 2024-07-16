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
		FSoftObjectPath TrapMeshPath;
		FSoftObjectPath TrapParticlePath = Elem.Value.TrapEffect.ToSoftObjectPath();
		FSoftObjectPath TrapAnimBlueprintPath = Elem.Value.TrapAnimBlueprint.ToSoftObjectPath();
		if (Elem.Value.TrapMeshType == ETrapMeshType::StaticMesh) {
			TrapMeshPath = Elem.Value.TrapStaticMesh.ToSoftObjectPath();
		}
		else {
			TrapMeshPath = Elem.Value.TrapSkeletalMesh.ToSoftObjectPath();
		}

		bool bAssetsValid = false;

		if (TrapMeshPath.IsValid())
		{
			TrapAssetsToLoad.Add(Elem.Value.TrapName, TrapMeshPath);
			bAssetsValid = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Trap Mesh path is invalid for %s"), *Elem.Value.TrapName.ToString());
		}

		if (TrapParticlePath.IsValid())
		{
			TrapAssetsToLoad.Add(Elem.Value.TrapName, TrapParticlePath);
			bAssetsValid = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Trap Particle path is invalid for %s"), *Elem.Value.TrapName.ToString());
		}

		if (TrapAnimBlueprintPath.IsValid())
		{
			TrapAssetsToLoad.Add(Elem.Value.TrapName, TrapAnimBlueprintPath);
			bAssetsValid = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Trap AnimBlueprint path is invalid for %s"), *Elem.Value.TrapName.ToString());
		}

		if (bAssetsValid)
		{
			UE_LOG(LogTemp, Warning, TEXT("Assets are valid for %s"), *Elem.Value.TrapName.ToString());
		}
	}

	// 비동기 로드 시작
	TArray<FSoftObjectPath> AssetsToLoad;
	TrapAssetsToLoad.GenerateValueArray(AssetsToLoad);
	MyGameInstance->GetStreamableManager().RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &UDDTrapAssetManager::OnTrapAssetsLoaded));
}

void UDDTrapAssetManager::OnTrapAssetsLoaded()
{
	UDDGameInstance* MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
	check(MyGameInstance);
	UE_LOG(LogTemp, Warning, TEXT("check OnTrapAssetsLoaded"));
	for (const TPair<FName, FSoftObjectPath>& AssetPair : TrapAssetsToLoad)
	{
		UObject* LoadedAsset = MyGameInstance->GetStreamableManager().LoadSynchronous(AssetPair.Value);

		if (UStaticMesh* LoadedStaticMesh = Cast<UStaticMesh>(LoadedAsset))
		{
			LoadedTrapStaticMeshes.Add(AssetPair.Key, LoadedStaticMesh);
			UE_LOG(LogTemp, Warning, TEXT("UStaticMesh loaded: %s"), *AssetPair.Key.ToString());
		}
		else if (USkeletalMesh* LoadedSkeletalMesh = Cast<USkeletalMesh>(LoadedAsset))
		{
			LoadedTrapSkeletalMeshes.Add(AssetPair.Key, LoadedSkeletalMesh);
			UE_LOG(LogTemp, Warning, TEXT("USkeletalMesh loaded: %s"), *AssetPair.Key.ToString());
		}
		else if (UAnimBlueprint* LoadedTrapAnimBlueprint = Cast<UAnimBlueprint>(LoadedAsset))
		{
			LoadedTrapAnimBlueprints.Add(AssetPair.Key, LoadedTrapAnimBlueprint);
			UE_LOG(LogTemp, Warning, TEXT("UAnimBlueprint loaded: %s"), *AssetPair.Key.ToString());
		}
		else if (UParticleSystem* LoadedTrapEffect = Cast<UParticleSystem>(LoadedAsset))
		{
			LoadedTrapEffects.Add(AssetPair.Key, LoadedTrapEffect);
			UE_LOG(LogTemp, Warning, TEXT("UParticleSystem loaded: %s"), *AssetPair.Key.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unsupported asset type loaded: %s"), *AssetPair.Key.ToString());
		}
	}


	// 로드 완료 처리
	TrapAssetsToLoad.Empty();
}

UStaticMesh* UDDTrapAssetManager::GetStaticMesh(const FName& TrapName)
{
	UStaticMesh* StaticMesh = *LoadedTrapStaticMeshes.Find(TrapName);
	return StaticMesh;
}

USkeletalMesh* UDDTrapAssetManager::GetSkeletalMesh(const FName& TrapName)
{
	USkeletalMesh* SkeletalMesh = *LoadedTrapSkeletalMeshes.Find(TrapName);
	return SkeletalMesh;
}

UAnimBlueprint* UDDTrapAssetManager::GetAnimBlueprint(const FName& TrapName)
{
	UAnimBlueprint* AnimBlueprint = *LoadedTrapAnimBlueprints.Find(TrapName);
	return AnimBlueprint;
}

UParticleSystem* UDDTrapAssetManager::GetParticleEffect(const FName& TrapName)
{
	UParticleSystem* ParticleSystem = *LoadedTrapEffects.Find(TrapName);
	return ParticleSystem;
}