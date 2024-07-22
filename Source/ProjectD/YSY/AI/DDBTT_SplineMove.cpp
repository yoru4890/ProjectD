// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDBTT_SplineMove.h"
#include "AIController.h"
#include "YSY/Interface/DDEnemyAIInterface.h"


UDDBTT_SplineMove::UDDBTT_SplineMove()
{
}

EBTNodeResult::Type UDDBTT_SplineMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IDDEnemyAIInterface* AIPawn = Cast<IDDEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Type();
}
