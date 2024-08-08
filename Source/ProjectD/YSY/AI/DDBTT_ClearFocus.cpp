// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDBTT_ClearFocus.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "YSY/AI/BlackboardKey.h"

UDDBTT_ClearFocus::UDDBTT_ClearFocus()
{
	NodeName = TEXT("ClearTarget");
}

EBTNodeResult::Type UDDBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	Controller->ClearFocus(EAIFocusPriority::Default);

	return EBTNodeResult::Succeeded;
}
