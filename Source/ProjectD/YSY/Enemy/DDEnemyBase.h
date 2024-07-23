// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YSY/GameData/DDEnemyData.h"
#include "YSY/Interface/DDEnemyAIInterface.h"
#include "DDEnemyBase.generated.h"

UCLASS()
class PROJECTD_API ADDEnemyBase : public ACharacter, public IDDEnemyAIInterface
{
	GENERATED_BODY()

public:
	ADDEnemyBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "DD")
	void InitializeEnemy(const FDDEnemyData& EnemyData);

	void SplineMoveFinish();

	void ArrivalAtGoal();

	void Die();
#pragma region AIInterface

	virtual void SplineMove() override;
	virtual void SetAIMoveFinishedDelegate(const FAISplineMoveFinished& InOnSplineMoveFinished) override;


#pragma endregion

private:

#pragma region FEnemyData

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
	float HealthWidgetHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	int32 GoldDropAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	uint8 bIsBoss : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	uint8 bIsElite : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	FString MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"))
	FString AnimationBlueprintPath;

#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DD", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class AAISplineRoute> AIMoveRoute;

	TObjectPtr<class ADDEnemyAIController> EnemyAIController;

	int32 RouteIndex;

	FAISplineMoveFinished OnSplineMoveFinished{};
};
