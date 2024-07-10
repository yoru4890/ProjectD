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
    TArray<FTrapStruct*> AllTrapData;
    TrapDataTable->GetAllRows(ContextString, AllTrapData);

    for (FTrapStruct* TrapStruct : AllTrapData)
    {
        if (TrapStruct)
        {
            FSoftObjectPath TrapMeshPath = TrapStruct->TrapMesh.ToSoftObjectPath();
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
        // 로드된 자산 사용
        UStaticMesh* LoadedMesh = Cast<UStaticMesh>(LoadedAsset);
        if (LoadedMesh)
        {
            LoadedTrapMeshes.Add(AssetPair.Key, LoadedMesh);
        }
    }

    // 로드 완료 처리
    TrapAssetsToLoad.Empty();
}

TObjectPtr<UStaticMesh> UDDTrapAssetManager::GetLoadedTrapMesh(const FName& TrapName) const
{
    if (LoadedTrapMeshes.Contains(TrapName))
    {
        return LoadedTrapMeshes[TrapName];
    }
    return nullptr;
}
