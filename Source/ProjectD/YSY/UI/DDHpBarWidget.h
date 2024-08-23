// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YSY/UI/DDUserWidget.h"
#include "DDHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDHpBarWidget : public UDDUserWidget
{
	GENERATED_BODY()
	
public:
	UDDHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(float InMaxHp);
	void UpdateHpBar(float NewCurrentHp);
	void SetVisiblePorgressBar(bool bIsVisible);
	void SetOwnerName(const FName& EnemyName);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> OwnerName;


	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
