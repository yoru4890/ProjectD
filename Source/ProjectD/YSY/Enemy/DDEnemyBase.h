// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DDEnemyBase.generated.h"

UCLASS()
class PROJECTD_API ADDEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADDEnemyBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
