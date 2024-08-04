// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YSY/GameData/DDEnemyData.h"
#include "YSY/Interface/DDEnemyAIInterface.h"
#include "YSY/Interface/DDCharacterWidgetInterface.h"
#include "YSY/Interface/DamageInterface.h"
#include "DDEnemyBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDieSignature, const FName&, ADDEnemyBase*);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDebuffSignature, FTimerHandle&, float, float);

struct FDotEffectState
{
	float DamageAmount = 0.0f;
	float ElapsedTime = 0.0f;
	FTimerHandle TimerHandle;
};

struct FDebuffState
{
	float AmountRate = 0.0f;
	float ElapsedTime = 0.0f;
	FTimerHandle TimerHandle;
	FOnDebuffSignature OnDebuffDelegate;
};

UCLASS()
class PROJECTD_API ADDEnemyBase : public ACharacter, public IDDEnemyAIInterface, public IDDCharacterWidgetInterface, public IDamageInterface
{
	GENERATED_BODY()

public:
	ADDEnemyBase();

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "DD")
	void InitializeEnemy(const FDDEnemyData& EnemyData);

	void SplineMoveFinish();

	void ArrivalAtGoal();

	void Die();

	void UpdateWidgetScale();

	void CaculateHackingDamage(float& ActualDamage);
	void CaculatePiercingDamage(float& ActualDamage);
	void CaculateCorrosionDamage(float& ActualDamage);

	void Activate();
	void Deactivate();

	void SetAIMoveRoute(TArray<class AAISplineRoute*> Splines, int32 Index);

	void ClearDotEffect(EDotDamageType DamageType);

	void ChangeMaxWalkSpeed(float Amount);
	void Stun(FTimerHandle& TimerHandle, float Time, float Amount);

	void BindingNotifyAttackFinished();

	UFUNCTION()
	void AttackFinished();

#pragma region AIInterface

	virtual void SplineMove() override;
	virtual void SetAIMoveFinishedDelegate(const FAISplineMoveOnFinishedSignature& InOnSplineMoveFinished) override;
	virtual void AttackByAI() override;
	virtual void SetAIAttackFinsihedDelegate(const FAIAttackOnFinishedSignature& InOnAttackFinished) override;

	virtual float GetAIDetectRange() const noexcept;
	virtual float GetAIAttackRange() const noexcept;
	virtual bool GetIsAggroState() const noexcept;
	virtual void SetIsAggroState(bool bNewAggroState);

#pragma endregion

#pragma region WidgetInterface

	virtual void SetupCharacterWidget(class UDDUserWidget* InUserWidget);

#pragma endregion

#pragma region DamageInterface

	UFUNCTION(BlueprintCallable)
	virtual float ApplyDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamageOverTime(EDotDamageType DamageType, float Time, float TimeInterval, float DamageAmount);
	UFUNCTION(BlueprintCallable)
	virtual void ApplyChainDamage(int DamageAmount, int NumberOfChain);
	UFUNCTION(BlueprintCallable)
	virtual void ApplyDebuff(EDebuffType DebuffType, float Time, float DebuffRate);

#pragma endregion

public:
	FOnDieSignature OnDie{};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDDEnemyStatComponent> Stat;
private:

#pragma region FEnemyData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	FName EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	TArray<FName> WeakPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	float AggroRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	int32 GoldDropAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	uint8 bIsBoss : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	uint8 bIsElite : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;

#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class AAISplineRoute> AIMoveRoute;

	UPROPERTY()
	TObjectPtr<class ADDEnemyAIController> EnemyAIController;

	int32 RouteIndex;

	FAISplineMoveOnFinishedSignature OnSplineMoveFinished{};
	FAIAttackOnFinishedSignature OnAttackFinished{};

	TMap<EDotDamageType, FDotEffectState> DotEffectStates;
	TMap<EDebuffType, FDebuffState> DebuffStates;

	bool bIsAggroState{};

	
};
