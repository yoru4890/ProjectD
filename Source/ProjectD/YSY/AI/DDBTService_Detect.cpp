// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDBTService_Detect.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/AI/BlackboardKey.h"
#include "YSY/Interface/DDEnemyAIInterface.h"
#include "YSY/Collision/CollisionChannel.h"
#include "YSY/Interface/AggroTargetInterface.h"

UDDBTService_Detect::UDDBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UDDBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	FVector Center{ ControllingPawn->GetActorLocation() };
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

	IAggroTargetInterface* AggroTarget = Cast<IAggroTargetInterface>(Target);

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float RangeWithRaduis = AIPawn->GetAIDetectRange();

	if (DistanceToTarget <= RangeWithRaduis)
	{
		bool bIsMaxAggro = AggroTarget->IsMaxAggro();

		if (!(AIPawn->GetIsAggroState()) && !bIsMaxAggro)
		{
			AIPawn->SetIsAggroState(true);
			AggroTarget->AddAggro();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISAGGRO, true);
		}
	}
	else
	{
		if (AIPawn->GetIsAggroState())
		{
			AIPawn->SetIsAggroState(false);
			AggroTarget->SubtractAggro();
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISAGGRO, false);
	}
}
