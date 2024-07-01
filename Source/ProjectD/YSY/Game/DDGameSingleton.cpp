// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDGameSingleton.h"

DEFINE_LOG_CATEGORY(LogDDGameSingleton);

UDDGameSingleton::UDDGameSingleton()
{
}

UDDGameSingleton& UDDGameSingleton::Get()
{
	UDDGameSingleton* Singleton = CastChecked<UDDGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogDDGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UDDGameSingleton>();
}
