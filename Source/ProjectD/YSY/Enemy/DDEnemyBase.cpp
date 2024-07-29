// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Enemy/DDEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "YSY/AI/DDEnemyAIController.h"
#include "YSY/AI/AISplineRoute.h"
#include "YSY/UI/DDHpBarWidget.h"
#include "YSY/UI/DDWidgetComponent.h"
#include "YSY/Stat/DDEnemyStatComponent.h"
#include "YSY/DamageType/AllDamageType.h"

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
	const UDamageType* DamageType = DamageEvent.DamageTypeClass.GetDefaultObject();

	float ActualDamage = DamageAmount;

	if (DamageType->IsA<UHackingDamageType>())
	{
		CaculateHackingDamage(ActualDamage);
	}
	else if (DamageType->IsA<UPiercingDamageType>())
	{
		CaculatePiercingDamage(ActualDamage);
	}
	else if (DamageType->IsA<UCorrosionDamageType>())
	{
		CaculateCorrosionDamage(ActualDamage);
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		if (PointDamageEvent->HitInfo.BoneName == "laser_01") // TODO : YSY WeakPoint Name
		{
			ActualDamage *= 1.5f; // TODO : YSY WeakPoint Caculation
			UE_LOG(LogTemp, Warning, TEXT("WeakPoint"));
		}
	}

	Stat->ApplyDamage(ActualDamage);

	return Super::TakeDamage(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
}

void ADDEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<ADDEnemyAIController>(GetController());
	check(EnemyAIController);

	EnemyAIController->OnMoveFinished.BindUObject(this, &ADDEnemyBase::SplineMoveFinish);
}

void ADDEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWidgetScale(); // TODO : YSY Doesn't work. Move to SetTimerEvent
}

void ADDEnemyBase::InitializeEnemy(const FDDEnemyData& EnemyData)
{

	EnemyName = EnemyData.EnemyName;
	WeakPoint = EnemyData.WeakPoint;
	EnemyType = EnemyData.EnemyType;
	MaxHP = EnemyData.MaxHP;
	MovementSpeed = EnemyData.MovementSpeed;
	AttackSpeed = EnemyData.AttackSpeed;
	Damage = EnemyData.Damage;
	AttackRange = EnemyData.AttackRange;
	AggroRange = EnemyData.AggroRange;
	//HealthWidgetHeight = EnemyData.HealthWidgetHeight; TODO : YSY Setting HpWidget Height
	GoldDropAmount = EnemyData.GoldDropAmount;
	bIsBoss = EnemyData.bIsBoss;
	bIsElite = EnemyData.bIsElite;

	USkeletalMesh* MeshTemp = LoadObject<USkeletalMesh>(nullptr, *EnemyData.MeshPath);
	ensure(MeshTemp);
	if (MeshTemp)
	{
		GetMesh()->SetSkeletalMesh(MeshTemp);
		GetMesh()->SetRelativeLocationAndRotation({ 0,0,-90 }, { 0,-90,0 });
	}

	UClass* AnimInstanceClass = LoadObject<UClass>(nullptr, *EnemyData.AnimationBlueprintPath);
	ensure(AnimInstanceClass);
	if (AnimInstanceClass)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	}
}

void ADDEnemyBase::SplineMove()
{
	FVector Destination = AIMoveRoute->GetSplinePointasWorldPosition(RouteIndex);
	RouteIndex++;
	EnemyAIController->MoveToLocation(Destination);
}

void ADDEnemyBase::SetAIMoveFinishedDelegate(const FAISplineMoveOnFinishedSignature& InOnSplineMoveFinished)
{
	OnSplineMoveFinished = InOnSplineMoveFinished;
}

void ADDEnemyBase::AttackByAI()
{
	// TODO : YSY Complete Attack

	UE_LOG(LogTemp, Warning, TEXT("Attack"));

	FTimerHandle AttackTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [this]()
		{
			OnAttackFinished.ExecuteIfBound();
		}, 0.1f, false, 2.0f);

	/*AActor* Player{};
	float DamageAmount{};
	FDamageEvent DamageEvent{};
	Player->TakeDamage(DamageAmount, DamageEvent, GetController(), this);*/
}

void ADDEnemyBase::SetAIAttackFinsihedDelegate(const FAIAttackOnFinishedSignature& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

float ADDEnemyBase::GetAIDetectRange() const noexcept
{
	// TODO : YSY Setting AIDetectRange

	return 350.0f;
}

float ADDEnemyBase::GetAIAttackRange() const noexcept
{
	// TODO : YSY Setting AILoseAggroRange

	return 200.0f;
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

void ADDEnemyBase::ApplyStun(float Time)
{
	// TODO : YSY ApplyStun
	UE_LOG(LogTemp, Warning, TEXT("Stun"));
}

void ADDEnemyBase::ApplySlow(float Time, float SlowRate)
{
	// TODO : YSY ApplySlow
	UE_LOG(LogTemp, Warning, TEXT("Slow"));
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

	OnDie.ExecuteIfBound(EnemyName, this);
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

// TODO : YSY Need to remove Magic Number

void ADDEnemyBase::CaculateHackingDamage(float& ActualDamage)
{
	// TODO : YSY HackingDamage
	UE_LOG(LogTemp, Warning, TEXT("Hacking"));

	switch (EnemyType)
	{
	case EEnemyType::LightArmor:
		ActualDamage *= 0.75f;
		break;
	case EEnemyType::Mechanical:
		ActualDamage *= 1.0f;
		break;
	case EEnemyType::Cybernetic:
		ActualDamage *= 2.0f;
		break;
	case EEnemyType::Unknown:
		UE_LOG(LogTemp, Warning, TEXT("UnknownDamage"));
		break;
	default:
		break;
	}

}

void ADDEnemyBase::CaculatePiercingDamage(float& ActualDamage)
{
	// TODO : YSY PiercingDamage
	UE_LOG(LogTemp, Warning, TEXT("Piercing"));

	switch (EnemyType)
	{
	case EEnemyType::LightArmor:
		ActualDamage *= 2.0f;
		break;
	case EEnemyType::Mechanical:
		ActualDamage *= 0.75f;
		break;
	case EEnemyType::Cybernetic:
		ActualDamage *= 1.0f;
		break;
	case EEnemyType::Unknown:
		UE_LOG(LogTemp, Warning, TEXT("UnknownDamage"));
		break;
	default:
		break;
	}
}

void ADDEnemyBase::CaculateCorrosionDamage(float& ActualDamage)
{
	// TODO : YSY CorrosionDamage
	UE_LOG(LogTemp, Warning, TEXT("Corrosion"));

	switch (EnemyType)
	{
	case EEnemyType::LightArmor:
		ActualDamage *= 1.0f;
		break;
	case EEnemyType::Mechanical:
		ActualDamage *= 2.0f;
		break;
	case EEnemyType::Cybernetic:
		ActualDamage *= 0.75f;
		break;
	case EEnemyType::Unknown:
		UE_LOG(LogTemp, Warning, TEXT("UnknownDamage"));
		break;
	default:
		break;
	}
}

void ADDEnemyBase::Activate()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	EnemyAIController->RunAI();
}

void ADDEnemyBase::Deactivate()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ADDEnemyBase::SetAIMoveRoute(TArray<class AAISplineRoute*> Splines, int32 Index)
{
	AIMoveRoute = Splines[Index];
}
