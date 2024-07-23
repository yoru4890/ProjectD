// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Enemy/DDEnemyBase.h"
#include "YSY/AI/DDEnemyAIController.h"
#include "YSY/AI/AISplineRoute.h"
#include "Components/CapsuleComponent.h"

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
	
	EnemyAIController = Cast<ADDEnemyAIController>(GetController());
	check(EnemyAIController);

	EnemyAIController->OnMoveFinished.BindUObject(this, &ADDEnemyBase::SplineMoveFinish);

	EnemyAIController->RunAI();

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

void ADDEnemyBase::SplineMove()
{
	FVector Destination = AIMoveRoute->GetSplinePointasWorldPosition(RouteIndex);
	
	EnemyAIController->MoveToLocation(Destination);
}

void ADDEnemyBase::SetAIMoveFinishedDelegate(const FAISplineMoveFinished& InOnSplineMoveFinished)
{
	OnSplineMoveFinished = InOnSplineMoveFinished;
}

void ADDEnemyBase::SplineMoveFinish()
{
	RouteIndex++;
	OnSplineMoveFinished.ExecuteIfBound();

	if (AIMoveRoute->IsSplineEnd(RouteIndex))
	{
		ArrivalAtGoal();
	}
}

void ADDEnemyBase::ArrivalAtGoal()
{
	GetMesh()->SetVisibility(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyAIController->StopAI();

	// TODO : YSY Caculate Potal Count
}

void ADDEnemyBase::Die()
{
	// TODO : Player get gold, Drop Item
}
