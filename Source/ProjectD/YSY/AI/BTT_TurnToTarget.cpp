// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/BTT_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "YSY/Interface/DDEnemyAIInterface.h"
#include "YSY/AI/BlackboardKey.h"

UBTT_TurnToTarget::UBTT_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTT_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	IDDEnemyAIInterface* AIPawn = Cast<IDDEnemyAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	/*if (AIPawn->NoTurn())
	{
		return EBTNodeResult::Failed;
	}*/

	float TurnSpeed = AIPawn->GetAITurnSpeed();
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
