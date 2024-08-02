// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/DDBuildingBaseStruct.h"
#include "UObject/NoExportTypes.h"
#include "LSM/DDLoadedAsset.h"
#include "DDAssetManager.generated.h"


UCLASS()
class PROJECTD_API UDDAssetManager : public UObject
{
	GENERATED_BODY()
	
public:
    void Initialize();

    void LoadAssetsAsync(const FName& RowName);

    FDDBuildingBaseStruct* GetLoadedAssetByName(const FName& RowName);

    void RemoveLoadedAssetByName(const FName& RowName);

    void RemoveLoadedAssetAll();

private:
    FDDBuildingBaseStruct* GetObjectBaseData(const FName& RowName);

    //// �ε��� Asset�� ��� �ִ� �ڷᱸ�� 
    //UPROPERTY()
    //TMap<FName, FDDLoadedAsset> LoadedAssets;
};
