// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DDEnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAISplineMoveOnFinishedSignature);
DECLARE_DELEGATE(FAIAttackOnFinishedSignature);

/**
 * 
 */
class PROJECTD_API IDDEnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void SplineMove() = 0;

	virtual void SetAIMoveFinishedDelegate(const FAISplineMoveOnFinishedSignature& InOnSplineMoveFinished) = 0;

	virtual void AttackByAI() = 0;

	virtual void SetAIAttackFinsihedDelegate(const FAIAttackOnFinishedSignature& InOnAttackFinished) = 0;

	virtual float GetAIDetectRange() const noexcept = 0;

	virtual float GetAILoseAggroRange() const noexcept = 0;
};
