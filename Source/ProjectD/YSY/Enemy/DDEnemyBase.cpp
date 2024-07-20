// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Enemy/DDEnemyBase.h"
#include "YSY/AI/DDEnemyAIController.h"

// Sets default values
ADDEnemyBase::ADDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ADDEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

void ADDEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADDEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADDEnemyBase::InitializeEnemy(const FDDEnemyData& EnemyData)
{
	EnemyType = EnemyData.EnemyType;
	MaxHP = EnemyData.MaxHP;
	MovementSpeed = EnemyData.MovementSpeed;
	AttackSpeed = EnemyData.AttackSpeed;
	Damage = EnemyData.Damage;
	AttackRange = EnemyData.AttackRange;
	AggroRange = EnemyData.AggroRange;
	HealthWidgetHeight = EnemyData.HealthWidgetHeight;
	GoldDropAmount = EnemyData.GoldDropAmount;
	bIsBoss = EnemyData.bIsBoss;
	bIsElite = EnemyData.bIsElite;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(*EnemyData.MeshPath);
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(*EnemyData.AnimationBlueprintPath);
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}

