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
	
protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UDDHpBarWidget* PlayerHealth;
};
