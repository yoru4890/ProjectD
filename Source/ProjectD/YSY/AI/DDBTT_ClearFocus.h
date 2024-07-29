// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DDBTT_ClearFocus.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDBTT_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UDDBTT_ClearFocus();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
