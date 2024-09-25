// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameMode.h"
#include "YSY/Game/DDPlayerState.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

ADDGameMode::ADDGameMode()
{
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassRef(TEXT("/Script/ProjectD.DDHUD"));

	if (HUDClassRef.Class)
	{
		HUDClass = HUDClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ProjectD.DDPlayerController"));

	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateClassRef(TEXT("/Script/ProjectD.DDGameState"));

	if (GameStateClassRef.Class)
	{
		GameStateClass = GameStateClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateClassRef(TEXT("/Script/ProjectD.DDPlayerState"));

	if (PlayerStateClassRef.Class)
	{
		PlayerStateClass = PlayerStateClassRef.Class;
	}

}