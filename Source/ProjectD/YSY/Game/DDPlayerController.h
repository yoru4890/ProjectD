// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADDPlayerController();
	UFUNCTION(BlueprintCallable)
	void ShowMainWidget();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetMainWidget();



protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> MainWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> MainWidget;
};
