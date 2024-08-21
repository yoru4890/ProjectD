// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/AI/DDBTService_CaculateDirection.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "YSY/AI/BlackboardKey.h"

UDDBTService_CaculateDirection::UDDBTService_CaculateDirection()
{
	NodeName = TEXT("CaculateDirection");
	Interval = 0.5f;
}

void UDDBTService_CaculateDirection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	FVector TargetDirection = (Target->GetActorLocation() - ControllingPawn->GetActorLocation()).GetSafeNormal2D();
	FVector ForwardDirection = ControllingPawn->GetActorForwardVector();

	double DotResult = FVector::DotProduct(TargetDirection, ForwardDirection);

	if (DotResult > 0.9)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISFRONT, true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISFRONT, false);
	}
}
