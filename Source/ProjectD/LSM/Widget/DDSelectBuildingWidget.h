// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDSelectBuildingWidget.generated.h"

/**
 * 
 */

 // 델리게이트 정의
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingSelected, FName, RowName);

UCLASS()
class PROJECTD_API UDDSelectBuildingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintCallable , Category = "Events")
	FOnBuildingSelected OnBuildingSelected;
};
