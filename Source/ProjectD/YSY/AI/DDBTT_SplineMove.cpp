// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDBTT_SplineMove.h"
#include "AIController.h"
#include "YSY/Interface/DDEnemyAIInterface.h"
#include "YSY/AI/AISplineRoute.h"
#include "Navigation/PathFollowingComponent.h"

DEFINE_LOG_CATEGORY(BTTLog);

UDDBTT_SplineMove::UDDBTT_SplineMove()
{
	NodeName = TEXT("Move Spline");
}

EBTNodeResult::Type UDDBTT_SplineMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	AAIController* AIController = Cast<AAIController>(ControllingPawn->GetController());
	if (nullptr == AIController)
	{
		return EBTNodeResult::Failed;
	}

	IDDEnemyAIInterface* AIPawn = Cast<IDDEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AISplineRoute = AIPawn->GetAISplineRoute();
	ensureMsgf(AISplineRoute, TEXT("AISplineRoute is NULL In BTT"));
	
	auto Destination = AISplineRoute->GetSplinePointasWorldPosition();
	auto MoveResult =AIController->MoveToLocation(Destination);
	
	if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
	{
		OwnerComptr = &OwnerComp;

		AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UDDBTT_SplineMove::OnMoveCompleted);
		AISplineRoute->IncrementRoute();
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UDDBTT_SplineMove::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.Code == EPathFollowingResult::Success)
	{
		FinishLatentTask(*OwnerComptr, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(*OwnerComptr, EBTNodeResult::Failed);
	}
}
