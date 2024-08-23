// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDBTService_CaculateRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "YSY/AI/BlackboardKey.h"
#include "YSY/Interface/DDEnemyAIInterface.h"

UDDBTService_CaculateRange::UDDBTService_CaculateRange()
{
	NodeName = TEXT("CaculateRange");
	Interval = 0.5f;
}

void UDDBTService_CaculateRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!Target)
	{
		return;
	}

	APawn* ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn)
	{
		return;
	}

	UWorld* World{ ControllingPawn->GetWorld() };
	if (!World)
	{
		return;
	}

	IDDEnemyAIInterface* AIPawn = Cast<IDDEnemyAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return;
	}

	if (AIPawn->GetIsDead())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CANATTACK, false);
		return;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float RangeWithRaduis = AIPawn->GetAIAttackRange();
	if (DistanceToTarget <= RangeWithRaduis)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CANATTACK, true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CANATTACK, false);
	}
}
