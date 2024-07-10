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


    // ������ ���̺��� ��� ���� ��ȸ
    static const FString ContextString(TEXT("GENERAL"));
    TArray<FTrapStruct*> AllTrapData;
    TrapDataTable->GetAllRows(ContextString, AllTrapData);

    for (FTrapStruct* TrapStruct : AllTrapData)
    {
        if (TrapStruct)
        {
            TrapAssetsToLoad.Add(TrapStruct->TrapName, TrapStruct->TrapMesh.ToSoftObjectPath());

            if (TrapStruct->TrapMesh)
            {
                UE_LOG(LogTemp, Log, TEXT("Trap Mesh is valid"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Trap Mesh is null"));
            }
        }
    }
   // // �ε��� Ʈ�� �ڻ� ��� ����
   // TrapAssetsToLoad = {
   //     FSoftObjectPath(TEXT("StaticMesh'/Game/Path/To/Mesh1.Mesh1'")),
   //     FSoftObjectPath(TEXT("StaticMesh'/Game/Path/To/Mesh2.Mesh2'")),
   //     // �ʿ��� ��ŭ �߰�
   // };

   // // �񵿱� �ε� ����
   ///StreamableManager.RequestAsyncLoad(TrapAssetsToLoad, FStreamableDelegate::CreateUObject(this, &UDDTrapAssetManager::OnTrapAssetsLoaded));
}

void UDDTrapAssetManager::OnTrapAssetsLoaded()
{
    //for (const FSoftObjectPath& AssetPath : TrapAssetsToLoad)
    //{
    //    UObject* LoadedAsset = StreamableManager.LoadSynchronous(AssetPath);
    //    // �ε�� �ڻ� ���
    //    UStaticMesh* LoadedMesh = Cast<UStaticMesh>(LoadedAsset);
    //    if (LoadedMesh)
    //    {
    //        LoadedTrapMeshes.Add(LoadedMesh);
    //    }
    //}

    //// �ε� �Ϸ� ó��
    //TrapAssetsToLoad.Empty();
}

TObjectPtr<UStaticMesh> UDDTrapAssetManager::GetLoadedTrapMesh(const FName& TowerName) const
{
    //if (LoadedTrapMeshes.IsValidIndex(Index))
    //{
    //    return LoadedTrapMeshes[Index];
    //}
    return nullptr;
}
