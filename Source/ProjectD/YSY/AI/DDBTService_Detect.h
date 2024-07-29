// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DDBTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDBTService_Detect : public UBTService
{
	GENERATED_BODY()
public:
	UDDBTService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
