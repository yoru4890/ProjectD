// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "DDBTT_SplineMove.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BTTLog, Log, All);

UCLASS()
class PROJECTD_API UDDBTT_SplineMove : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UDDBTT_SplineMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UBehaviorTreeComponent* OwnerComptr;

	class AAISplineRoute* AISplineRoute;
};
