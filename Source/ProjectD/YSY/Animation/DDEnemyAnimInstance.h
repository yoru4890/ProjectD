// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DDEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UDDEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UDDEnemyAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Character")
	class ADDEnemyBase* owner{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Character")
	class UCharacterMovementComponent* charMoveComp{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Character")
	FVector velocity{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Character")
	float moveSpeed{};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy|Character")
	float angle{};
};
