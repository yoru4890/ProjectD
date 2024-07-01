// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDPlayerController.h"

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
