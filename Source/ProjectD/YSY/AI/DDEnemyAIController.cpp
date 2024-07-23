// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "YSY/AI/BlackboardKey.h"

ADDEnemyAIController::ADDEnemyAIController()
{
	ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/0000/YSY/AI/BB_DDEnemy.BB_DDEnemy'"));

	if (BBFinder.Object)
	{
		ownedBB = BBFinder.Object;
	}

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/0000/YSY/AI/BT_DDEnemy.BT_DDEnemy'"));

	if (BTFinder.Object)
	{
		ownedBT = BTFinder.Object;
	}

	ReceiveMoveCompleted.AddDynamic(this, &ADDEnemyAIController::OnMoveCompleted);
}

void ADDEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ADDEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ADDEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	OnMoveFinished.ExecuteIfBound();
}

void ADDEnemyAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr{ Blackboard.Get() };
	if (UseBlackboard(ownedBB, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(ownedBT);
		ensure(RunResult);

	}
}

void ADDEnemyAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}
