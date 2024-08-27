// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDPlayerController.h"
#include "Blueprint/UserWidget.h"

ADDPlayerController::ADDPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/YSY/Widget/YSY_WBP_Main.YSY_WBP_Main_C'"));

	if (MainWidgetClassRef.Class)
	{
		MainWidgetClass = MainWidgetClassRef.Class;
	}
}

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	MainWidget = CreateWidget(GetWorld(), MainWidgetClass);
	MainWidget->AddToViewport();
}
