// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DDEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	ADDEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
public:
	void RunAI();
	void StopAI();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBlackboardData> ownedBB{};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBehaviorTree> ownedBT{};
};
