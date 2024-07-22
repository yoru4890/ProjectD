// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DDPEngineerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDPEngineerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public :
	UDDPEngineerAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
