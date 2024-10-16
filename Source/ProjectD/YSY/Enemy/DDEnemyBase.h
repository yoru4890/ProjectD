// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YSY/GameData/DDEnemyData.h"
#include "YSY/Interface/DDEnemyAIInterface.h"
#include "YSY/Interface/DDCharacterWidgetInterface.h"
#include "YSY/Interface/DamageInterface.h"
#include "YSY/Interface/EnemyHpVisibleInterface.h"
#include "DDEnemyBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDieSignature, const FName&, ADDEnemyBase*);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDebuffSignature, FTimerHandle&, float, float);

DECLARE_DELEGATE_OneParam(FOnSetVisibleHpBarSignature, bool);

DECLARE_DELEGATE_OneParam(FOnSubRemainingLivesSignature, int32);

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
class PROJECTD_API ADDEnemyBase : public ACharacter, public IDDEnemyAIInterface, public IDDCharacterWidgetInterface, public IDamageInterface, public IEnemyHpVisibleInterface
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

	void BindingAnimNotify();

	UFUNCTION()
	void AttackFinished();

	UFUNCTION()
	void MeleeAttack();

	UFUNCTION()
	void RangeAttack();

	void ShowHpBarbyAttack();

	UFUNCTION()
	void PlayAttackEffect();

	UFUNCTION()
	void PlayDeathEffect();

#pragma region AIInterface

	virtual void SplineMove() override;
	virtual void SetAIMoveFinishedDelegate(const FAISplineMoveOnFinishedSignature& InOnSplineMoveFinished) override;
	virtual void AttackByAI() override;
	virtual void SetAIAttackFinsihedDelegate(const FAIAttackOnFinishedSignature& InOnAttackFinished) override;

	virtual float GetAIDetectRange() const noexcept;
	virtual float GetAIAttackRange() const noexcept;
	virtual bool GetIsAggroState() const noexcept;
	virtual void SetIsAggroState(bool bNewAggroState);

	virtual float GetAITurnSpeed() const noexcept;

	virtual bool GetIsDead() const noexcept { return bIsDead; }

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

#pragma region EnemyHpVisibleInterface

	virtual void SetVisibleHpBar(bool bIsVisible) override;

#pragma endregion

public:
	FOnDieSignature OnDie{};
	FOnSubRemainingLivesSignature OnSubRemainingLivesSignature{};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDDEnemyStatComponent> Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> AttackAudio;
private:

#pragma region FEnemyData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	FName EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	TArray<FName> WeakPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	EEnemyAttackType EnemyAttackType;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	TArray<FEffectData> AttackEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	TArray<FEffectData> DeathEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	float SoundStartTime;

#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class AAISplineRoute> AIMoveRoute;

	UPROPERTY()
	TObjectPtr<class ADDEnemyAIController> EnemyAIController;

	int32 RouteIndex;

	FAISplineMoveOnFinishedSignature OnSplineMoveFinished{};
	FAIAttackOnFinishedSignature OnAttackFinished{};

	FOnSetVisibleHpBarSignature OnSetVisibleHpBarDelegate{};

	TMap<EDotDamageType, FDotEffectState> DotEffectStates;
	TMap<EDebuffType, FDebuffState> DebuffStates;

	bool bIsAggroState{};
	bool bIsCanTurn{ true };
	bool bIsDead{};

	float TurnSpeed{ 5.0f }; // TODO : YSY Magic Number, DataTable

	UPROPERTY()
	TObjectPtr<AActor> Player;

	FTimerHandle HpBarTH{};

	UPROPERTY()
	TArray<class UNiagaraComponent*> NiagaraAttackEffect;
};
