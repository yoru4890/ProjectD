// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDRadialWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDRadialWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Radial 버튼을 동적으로 생성하고 배치
    UFUNCTION(BlueprintCallable, Category = "RadialMenu")
    void CreateRadialMenu(float Radius);

    UPROPERTY(EditAnyWhere)
    int32 ButtonCount;

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnRadialButtonClicked();

    // 필수 버튼들
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TArray<TObjectPtr<class UButton>> PrimaryButtons;

};