// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapManager/DDTrapAssetManager.h"
#include "Engine/StreamableManager.h"
#include "YSY/Game/DDGameSingleton.h"
#include "YSY/Game/DDGameInstance.h"
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
        if (Elem.Value.TrapMeshType == ETrapMeshType::StaticMesh) {
            TrapMeshPath = Elem.Value.TrapStaticMesh.ToSoftObjectPath();
        }
        else {
            TrapMeshPath = Elem.Value.TrapSkeletalMesh.ToSoftObjectPath();
        }
            
        if (TrapMeshPath.IsValid())
        {
            TrapAssetsToLoad.Add(Elem.Value.TrapName, TrapMeshPath);
            UE_LOG(LogTemp, Warning, TEXT("Trap Mesh is valid for %s"), *Elem.Value.TrapName.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Trap Mesh path is invalid for %s"), *Elem.Value.TrapName.ToString());
        }
    }

    // 비동기 로드 시작
    TArray<FSoftObjectPath> AssetsToLoad;
    TrapAssetsToLoad.GenerateValueArray(AssetsToLoad);
    MyGameInstance->GetStreamableManager().RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &UDDTrapAssetManager::OnTrapAssetsLoaded));
}

void UDDTrapAssetManager::OnTrapAssetsLoaded()
{
    TObjectPtr<UDDGameInstance> MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
    check(MyGameInstance);

    for (const TPair<FName, FSoftObjectPath>& AssetPair : TrapAssetsToLoad)
    {
        UObject* LoadedAsset = MyGameInstance->GetStreamableManager().LoadSynchronous(AssetPair.Value);

        if (UStaticMesh* LoadedStaticMesh = Cast<UStaticMesh>(LoadedAsset))
        {
            LoadedTrapStaticMeshes.Add(AssetPair.Key, LoadedStaticMesh);
        }
        else if (USkeletalMesh* LoadedSkeletalMesh = Cast<USkeletalMesh>(LoadedAsset))
        {
            LoadedTrapSkeletalMeshes.Add(AssetPair.Key, LoadedSkeletalMesh);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Unsupported asset type loaded: %s"), *AssetPair.Key.ToString());
        }
    }


    // 로드 완료 처리
    TrapAssetsToLoad.Empty();
}
