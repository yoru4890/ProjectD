// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YSY/UI/DDUserWidget.h"
#include "DDMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDMainWidget : public UDDUserWidget
{
	GENERATED_BODY()
	
public:
    FORCEINLINE class UDDPlayerHPBarWidget* GetHpBarWidget() const noexcept { return PlayerHealth; }

	UFUNCTION(BlueprintCallable)
	void SetWaveText(int32 CurrentWave, int32 MaxWave);
	UFUNCTION(BlueprintCallable)
	void SetGoldText(int32 CurrentGold);

	UFUNCTION(BlueprintCallable)
	void SetRifleAmmoText(int32 CurrentLoadedAmmo, int32 CurrentUnLoadedAmmo);
	UFUNCTION(BlueprintCallable)
	void GetRifleAmmoFromText(int32& OutLoadedAmmo, int32& OutUnLoadedAmmo);
	UFUNCTION(BlueprintCallable)
	void SetLoadedRifleAmmoText(int32 CurrentLoadedAmmo);
	UFUNCTION(BlueprintCallable)
	void SetUnLoadedRifleAmmoText(int32 CurrentUnLoadedAmmo);
	UFUNCTION(BlueprintCallable)
	void SetRemainingLivesText(int32 CurrentLives);
	UFUNCTION(BlueprintCallable)
	void SetVisibilityAmmoText(bool IsVisible);


protected:
    virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> WaveText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> GoldText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> RifleAmmoText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> RemainingLivesText;

private:
    UPROPERTY(meta = (BindWidget))
    class UDDPlayerHPBarWidget* PlayerHealth;
};
