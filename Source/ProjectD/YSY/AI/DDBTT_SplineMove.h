// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DDBTT_SplineMove.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDBTT_SplineMove : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UDDBTT_SplineMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
