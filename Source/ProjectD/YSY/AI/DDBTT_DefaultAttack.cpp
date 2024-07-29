// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDBTT_DefaultAttack.h"
#include "AIController.h"
#include "YSY/Interface/DDEnemyAIInterface.h"

UDDBTT_DefaultAttack::UDDBTT_DefaultAttack()
{
	NodeName = TEXT("DefaultAttack");
}

EBTNodeResult::Type UDDBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IDDEnemyAIInterface* AIPawn = Cast<IDDEnemyAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAIAttackOnFinishedSignature OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackFinsihedDelegate(OnAttackFinished);
	AIPawn->AttackByAI();

	return EBTNodeResult::InProgress;
}
