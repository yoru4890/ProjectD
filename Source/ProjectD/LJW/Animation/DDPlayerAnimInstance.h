// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DDPlayerAnimInstance.generated.h"


UCLASS()
class PROJECTD_API UDDPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public :
	UDDPlayerAnimInstance();

	FORCEINLINE bool GetIsAiming() const noexcept { return bIsAiming; }
	void SetIsAiming(bool NewbIsAiming) { bIsAiming = NewbIsAiming; }

	FORCEINLINE bool GetIsDead() const noexcept { return bIsDead; }
	void SetIsDead(bool NewbIsDead) { bIsDead = NewbIsDead;  }

	FORCEINLINE int32 GetWeaponIndex() const noexcept { return WeaponIndex; }
	void SetWeaponIndex(int32 Index) { WeaponIndex = Index; UE_LOG(LogTemp, Warning, TEXT("WeaponIndex : %d"), WeaponIndex); }

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDead : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	int32 WeaponIndex;
};
