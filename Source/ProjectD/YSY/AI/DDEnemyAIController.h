// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "DDEnemyAIController.generated.h"

DECLARE_DELEGATE(FMoveOnFinishedSignature);

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

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;


public:
	void RunAI();
	void StopAI();

public:
	FMoveOnFinishedSignature OnMoveFinished;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBlackboardData> ownedBB{};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBehaviorTree> ownedBT{};

};
