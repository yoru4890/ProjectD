// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Enemy/DDEnemyBase.h"
#include "YSY/AI/DDEnemyAIController.h"
#include "YSY/AI/AISplineRoute.h"
#include "Components/CapsuleComponent.h"
#include "YSY/UI/DDHpBarWidget.h"
#include "YSY/UI/DDWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "YSY/Stat/DDEnemyStatComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
ADDEnemyBase::ADDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ADDEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	HpBar = CreateDefaultSubobject<UDDWidgetComponent>(TEXT("HpWidget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	Stat = CreateDefaultSubobject<UDDEnemyStatComponent>(TEXT("Stat"));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/0000/YSY/Widget/YSY_WBP_EnemyHpBar.YSY_WBP_EnemyHpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ADDEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &ADDEnemyBase::Die);
}

float ADDEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	return ResultDamage;
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

	UpdateWidgetScale(); // TODO : YSY Doesn't work. Move to SetTimerEvent
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
	RouteIndex++;
	EnemyAIController->MoveToLocation(Destination);
}

void ADDEnemyBase::SetAIMoveFinishedDelegate(const FAISplineMoveFinished& InOnSplineMoveFinished)
{
	OnSplineMoveFinished = InOnSplineMoveFinished;
}

void ADDEnemyBase::SetupCharacterWidget(UDDUserWidget* InUserWidget)
{
	UDDHpBarWidget* HpBarWidget = Cast<UDDHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(100.0f); // TODO : YSY Setting MaxHp
		HpBarWidget->UpdateHpBar(100.0f); // TODO : YSY StatComponent
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UDDHpBarWidget::UpdateHpBar);
		// TODO : YSY StatComponent
	}
}

void ADDEnemyBase::SplineMoveFinish()
{
	OnSplineMoveFinished.ExecuteIfBound();

	if (AIMoveRoute->IsSplineEnd(RouteIndex))
	{
		ArrivalAtGoal();
	}
}

void ADDEnemyBase::ArrivalAtGoal()
{
	GetMesh()->SetVisibility(false, true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyAIController->StopAI();

	// TODO : YSY Caculate Potal Count
}

void ADDEnemyBase::Die()
{
	// TODO : YSY Player get gold, Drop Item
}

void ADDEnemyBase::UpdateWidgetScale()
{
	if (HpBar)
	{
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		float Distance = FVector::Dist(CameraLocation, GetActorLocation());
		float ScaleFactor = FMath::Clamp(Distance / 1000.0f, 0.5f, 2.0f);
		HpBar->SetWorldScale3D(FVector(ScaleFactor));
	}
}
