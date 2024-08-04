// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DDTrapAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDTrapAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UDDTrapAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayAnimationMontage(UAnimMontage* InMontage);

private:
	void OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted);

	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentMontage;
};
