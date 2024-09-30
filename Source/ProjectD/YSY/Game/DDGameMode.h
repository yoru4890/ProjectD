// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ADDGameMode();
	
	void InitializePoolAsync(TFunction<void()> OnComplete);
	UFUNCTION(BlueprintCallable)
	void StageStart(const int32& NewCurrentStage);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> LoadingWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentStage;
};
