// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Enemy/DDEnemyBase.h"
#include "YSY/AI/DDEnemyAIController.h"

// Sets default values
ADDEnemyBase::ADDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ADDEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ADDEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADDEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

