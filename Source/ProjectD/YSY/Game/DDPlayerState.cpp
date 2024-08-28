// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Game/DDPlayerState.h"

ADDPlayerState::ADDPlayerState()
{
	Gold = 10000;
	LikePoint = 5;
}

void ADDPlayerState::AddGold(const int32 InGold)
{
	Gold += InGold;
	UE_LOG(LogTemp, Warning, TEXT("Gold Added"));
	OnGoldChanged.Broadcast(Gold);  // Gold가 변경될 때 델리게이트 호출
}

bool ADDPlayerState::SubtractGold(const int32 InGold)
{
	if (CheckGold(InGold))
	{
		Gold -= InGold;
		UE_LOG(LogTemp, Warning, TEXT("Gold Used"));
		OnGoldChanged.Broadcast(Gold);  // Gold가 변경될 때 델리게이트 호출
		return true;
	}
	else
	{
		return false;
	}
}

void ADDPlayerState::AddLikePoint(const int32 InLikePoint)
{
	LikePoint += InLikePoint;
	OnLikePointChanged.Broadcast(LikePoint);  // LikePoint가 변경될 때 델리게이트 호출
}

bool ADDPlayerState::SubtractLikePoint(const int32 InLikePoint)
{
	if (CheckLikePoint(InLikePoint))
	{
		LikePoint -= InLikePoint;
		OnLikePointChanged.Broadcast(LikePoint);  // LikePoint가 변경될 때 델리게이트 호출
		return true;
	}
	else
	{
		return false;
	}
}