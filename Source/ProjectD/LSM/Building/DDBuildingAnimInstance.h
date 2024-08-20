// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DDBuildingAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDBuildingAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UDDBuildingAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayAnimationMontage(UAnimMontage* InMontage);

	FORCEINLINE void SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor;}
	FORCEINLINE void SetIsRotateToTarget(const bool InRotateToTarget) { IsRotateToTarget = InRotateToTarget; }

private:
	void OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentMontage;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsRotateToTarget = false;
};
