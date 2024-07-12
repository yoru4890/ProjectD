// Fill out your copyright notice in the Description page of Project Settings.


#include "LSM/TrapManager/DDTrapAssetManager.h"
#include "LSM/TrapManager/DDTrapManager.h"
#include "YSY/Game/DDGameInstance.h"
#include "Engine/AssetManager.h"


UDDTrapAssetManager::UDDTrapAssetManager()
{

}

void UDDTrapAssetManager::LoadTrapAssetsAsync()
{
    TObjectPtr<UDDGameInstance> MyGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
    check(MyGameInstance);

    TObjectPtr<UDDTrapManager> TrapManager = MyGameInstance->GetTrapManager();
    check(TrapManager);
   
    TObjectPtr<UDataTable> TrapDataTable = TrapManager->GetTrapDataTable();
    check(TrapDataTable);


    // 데이터 테이블의 모든 행을 순회
    static const FString ContextString(TEXT("GENERAL"));
    TArray<FDDTrapStruct*> AllTrapData;
    TrapDataTable->GetAllRows(ContextString, AllTrapData);

    for (FDDTrapStruct* TrapStruct : AllTrapData)
    {
        if (TrapStruct)
        {
            FSoftObjectPath TrapMeshPath;
            if (TrapStruct->TrapMeshType == ETrapMeshType::StaticMesh) {
                TrapMeshPath = TrapStruct->TrapStaticMesh.ToSoftObjectPath();
            }
            else {
                TrapMeshPath = TrapStruct->TrapSkeletalMesh.ToSoftObjectPath();
            }
            
            if (TrapMeshPath.IsValid())
            {
                TrapAssetsToLoad.Add(TrapStruct->TrapName, TrapMeshPath);
                UE_LOG(LogTemp, Log, TEXT("Trap Mesh is valid for %s"), *TrapStruct->TrapName.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Trap Mesh path is invalid for %s"), *TrapStruct->TrapName.ToString());
            }
        }
    }

    // 비동기 로드 시작
    TArray<FSoftObjectPath> AssetsToLoad;
    TrapAssetsToLoad.GenerateValueArray(AssetsToLoad);
    StreamableManager.RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &UDDTrapAssetManager::OnTrapAssetsLoaded));
}

void UDDTrapAssetManager::OnTrapAssetsLoaded()
{
    for (const TPair<FName, FSoftObjectPath>& AssetPair : TrapAssetsToLoad)
    {
        UObject* LoadedAsset = StreamableManager.LoadSynchronous(AssetPair.Value);

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

TObjectPtr<UStaticMesh> UDDTrapAssetManager::GetLoadedTrapStaticMesh(const FName& TrapName) const
{
    if (LoadedTrapStaticMeshes.Contains(TrapName))
    {
        return LoadedTrapStaticMeshes[TrapName];
    }
    return nullptr;
}

TObjectPtr<USkeletalMesh> UDDTrapAssetManager::GetLoadedTrapSkeletalMesh(const FName& TrapName) const
{
    if (LoadedTrapSkeletalMeshes.Contains(TrapName))
    {
        return LoadedTrapSkeletalMeshes[TrapName];
    }
    return nullptr;
}
