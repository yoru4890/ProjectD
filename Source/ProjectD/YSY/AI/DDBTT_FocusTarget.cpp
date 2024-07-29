// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDBTT_FocusTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "YSY/AI/BlackboardKey.h"

UDDBTT_FocusTarget::UDDBTT_FocusTarget()
{
	NodeName = TEXT("FocusTarget");
}

EBTNodeResult::Type UDDBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = Controller->GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(BBKEY_TARGET));
	ensure(Target);

	Controller->SetFocus(Target);

	return EBTNodeResult::Succeeded;
}
