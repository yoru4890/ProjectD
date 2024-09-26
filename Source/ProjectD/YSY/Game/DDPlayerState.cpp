// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/Game/DDGameInstance.h"
#include "LSM/Manager/DDBuildingManager.h"

ADDPlayerState::ADDPlayerState()
{
	Gold = 10000;
	LikePoint = 5;
	UnLoadedRifleAmmo = 60;
}

void ADDPlayerState::AddGold(const int32 InGold)
{
	Gold += InGold;
	UE_LOG(LogTemp, Warning, TEXT("Gold Added"));
	OnGoldChanged.Broadcast(Gold);  // Gold가 변경될 때 델리게이트 호출
}

bool ADDPlayerState::SubtractGold(const int32 InGold)
{
	if (CheckGold(InGold))
	{
		Gold -= InGold;
		UE_LOG(LogTemp, Warning, TEXT("Gold Used"));
		OnGoldChanged.Broadcast(Gold);  // Gold가 변경될 때 델리게이트 호출
		return true;
	}
	else
	{
		return false;
	}
}

void ADDPlayerState::AddLikePoint(const int32 InLikePoint)
{
	LikePoint += InLikePoint;
	if (!DDSaveGame)
	{
		CreateSaveFile("SaveFile");
		UE_LOG(LogTemp, Warning, TEXT("Create Save File, Because of None SaveFile"));
	}
	DDSaveGame->SetCurrentLikePoint(LikePoint);
	OnLikePointChanged.Broadcast(LikePoint);  // LikePoint가 변경될 때 델리게이트 호출
}

bool ADDPlayerState::SubtractLikePoint(const int32 InLikePoint)
{
	if (CheckLikePoint(InLikePoint))
	{
		LikePoint -= InLikePoint;
		if (!DDSaveGame) 
		{
			CreateSaveFile("SaveFile");
			UE_LOG(LogTemp, Warning, TEXT("Create Save File, Because of None SaveFile"));
		}
		DDSaveGame->SetCurrentLikePoint(LikePoint);
		OnLikePointChanged.Broadcast(LikePoint);	// LikePoint가 변경될 때 델리게이트 호출
		return true;
	}
	else
	{
		return false;
	}
}

//void ADDPlayerState::AddLoadedRifleAmmo()
//{
//	const int32 MaxAmmoPerMagazine = 30;
//
//	int32 AmmoToLoad = FMath::Min(UnLoadedRifleAmmo, MaxAmmoPerMagazine - LoadedRifleAmmo);
//	LoadedRifleAmmo += AmmoToLoad;
//
//	OnLoadedAmmoChanged.Broadcast(LoadedRifleAmmo);
//	
//	SubtractUnloadedRifleAmmo(AmmoToLoad);
//}
//
//bool ADDPlayerState::SubtractLoadedRifleAmmo()
//{
//	if (LoadedRifleAmmo > 0)
//	{
//		LoadedRifleAmmo--;
//		OnLoadedAmmoChanged.Broadcast(LoadedRifleAmmo);
//		return true;
//	}
//	return false;
//}
//
//void ADDPlayerState::AddUnloadedRifleAmmo(const int32 InAmmo)
//{
//	const int32 MaxUnLoadedAmmo = 120;
//
//	if (UnLoadedRifleAmmo + InAmmo > MaxUnLoadedAmmo)
//	{
//		UnLoadedRifleAmmo = MaxUnLoadedAmmo;
//	}
//	else
//	{
//		UnLoadedRifleAmmo += InAmmo;
//	}
//
//	OnUnLoadedAmmoChanged.Broadcast(UnLoadedRifleAmmo);
//}
//
//bool ADDPlayerState::SubtractUnloadedRifleAmmo(const int32 InAmmo)
//{
//	if (UnLoadedRifleAmmo >= InAmmo)
//	{
//		UnLoadedRifleAmmo -= InAmmo;
//		OnUnLoadedAmmoChanged.Broadcast(UnLoadedRifleAmmo);
//		return true;
//	}
//	return false;
//}

void ADDPlayerState::AutoSaveGame()
{
	UGameplayStatics::SaveGameToSlot(DDSaveGame, DDSaveGame->GetSaveSlotName(), 0);
}

void ADDPlayerState::CreateSaveFile(const FString& SlotName)
{
	DDSaveGame = Cast<UDDSaveGame>(UGameplayStatics::CreateSaveGameObject(UDDSaveGame::StaticClass()));
	UGameplayStatics::SaveGameToSlot(DDSaveGame, SlotName, 0);
}

void ADDPlayerState::SaveGame(const FString& SlotName)
{
	DDSaveGame->SetSaveSlotName(SlotName);
	UGameplayStatics::SaveGameToSlot(DDSaveGame, SlotName, 0);
}

void ADDPlayerState::LoadGame(const FString& SlotName)
{
	UDDSaveGame* TempSaveGame = Cast<UDDSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (TempSaveGame)
	{
		DDSaveGame = TempSaveGame;
		DDSaveGame->SetSaveSlotName(SlotName);
		LikePoint = DDSaveGame->GetCurrentLikePoint();

		auto TempGameInstance = Cast<UDDGameInstance>(GetWorld()->GetGameInstance());
		auto TempBuildingManager = TempGameInstance->GetBuildingManager();
		TempBuildingManager->SaveUnlockBuilding(DDSaveGame->GetBuildingUnlocked());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There's not SaveSlot: %s"), *SlotName);
	}
}

void ADDPlayerState::UpdateBuildingLockState(const FName& BuildingName, const bool& bIsUnlocked)
{
	DDSaveGame->SetBuildingLockState(BuildingName, bIsUnlocked);
}

void ADDPlayerState::UpdateStageLikePoint(const int32& CurrentStage, const int32& InLikePoint)
{
	DDSaveGame->SetStageLikePoint(CurrentStage, InLikePoint);
}

void ADDPlayerState::UpdateStageCleared(const int32& CurrentStage, const bool& bIsCleared)
{
	DDSaveGame->SetStageCleared(CurrentStage, bIsCleared);
}
