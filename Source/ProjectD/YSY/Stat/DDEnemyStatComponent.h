// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DDEnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTD_API UDDEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDDEnemyStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	FORCEINLINE float GetCurrentHp() const noexcept { return CurrentHp; }
	void SetCurrentHp(float NewHp);
		
	float ApplyDamage(float InDamage);

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "DD")
	float MaxHp;
};
