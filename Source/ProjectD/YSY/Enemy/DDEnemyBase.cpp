// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Enemy/DDEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	ensure(Stat);

	Stat->OnHpZero.AddUObject(this, &ADDEnemyBase::Die);
	Stat->OnMovementSpeedChange.AddUObject(this, &ADDEnemyBase::ChangeMaxWalkSpeed);

	DebuffStates.Add(EDebuffType::Stun, FDebuffState());
	DebuffStates.Add(EDebuffType::Slow, FDebuffState());
	DebuffStates.Add(EDebuffType::DamageIncrease, FDebuffState());

	if (Stat)
	{
		DebuffStates[EDebuffType::Stun].OnDebuffDelegate.AddUObject(this, &ADDEnemyBase::Stun);
		DebuffStates[EDebuffType::Slow].OnDebuffDelegate.AddUObject(Stat, &UDDEnemyStatComponent::ApplySlow);
		DebuffStates[EDebuffType::DamageIncrease].OnDebuffDelegate.AddUObject(Stat, &UDDEnemyStatComponent::ApplyDamageReceiveIncrease);
	}
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

		if (PointDamageEvent->HitInfo.BoneName == WeakPoint)
		{
			ActualDamage *= 1.5f; // TODO : YSY WeakPoint Caculation
			UE_LOG(LogTemp, Warning, TEXT("WeakPoint"));
		}
	}

	Stat->ApplyStatDamage(ActualDamage * Stat->GetDamageReceiveRate());
	UE_LOG(LogTemp, Warning, TEXT("%f"), ActualDamage * Stat->GetDamageReceiveRate());
	return ActualDamage;
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
	Stat->SetMaxHp(MaxHP);
	Stat->SetCurrentHp(MaxHP);
	MovementSpeed = EnemyData.MovementSpeed;
	AttackSpeed = EnemyData.AttackSpeed;
	Damage = EnemyData.Damage;
	AttackRange = EnemyData.AttackRange;
	AggroRange = EnemyData.AggroRange;
	HpBar->SetRelativeLocation({ 0.0f,0.0f,EnemyData.HealthWidgetHeight });
	GoldDropAmount = EnemyData.GoldDropAmount;
	bIsBoss = EnemyData.bIsBoss;
	bIsElite = EnemyData.bIsElite;

	USkeletalMesh* MeshTemp = LoadObject<USkeletalMesh>(nullptr, *EnemyData.MeshPath);
	ensure(MeshTemp);
	if (MeshTemp)
	{
		GetMesh()->SetSkeletalMesh(MeshTemp);
		GetMesh()->SetRelativeLocationAndRotation({ 0,0,-90 }, { 0,-90,0 });
		GetMesh()->SetCollisionProfileName(FName("Enemy"));
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
	return AggroRange;
}

float ADDEnemyBase::GetAIAttackRange() const noexcept
{
	return AttackRange;
}

bool ADDEnemyBase::GetIsAggroState() const noexcept
{
	return bIsAggroState;
}

void ADDEnemyBase::SetIsAggroState(bool bNewAggroState)
{
	bIsAggroState = bNewAggroState;
}

void ADDEnemyBase::SetupCharacterWidget(UDDUserWidget* InUserWidget)
{
	UDDHpBarWidget* HpBarWidget = Cast<UDDHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetMaxHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UDDHpBarWidget::UpdateHpBar);
		// TODO : YSY StatComponent
	}
}

float ADDEnemyBase::ApplyDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ADDEnemyBase::ApplyDamageOverTime(EDotDamageType DamageType, float Time, float TimeInterval, float DamageAmount)
{
	FDotEffectState& DotEffectState = DotEffectStates.FindOrAdd(DamageType);

	
	if (DotEffectState.ElapsedTime > 0.0f && DamageAmount < DotEffectState.DamageAmount)
	{
		return;
	}
	
	DotEffectState.ElapsedTime = 0.0f;
	DotEffectState.DamageAmount = DamageAmount;
	
	GetWorld()->GetTimerManager().SetTimer(DotEffectState.TimerHandle, [this, &DotEffectState, TimeInterval, Time, DamageType]()
		{
			Stat->ApplyStatDamage(DotEffectState.DamageAmount * Stat->GetDamageReceiveRate());
			DotEffectState.ElapsedTime += TimeInterval;
			if (DotEffectState.ElapsedTime >= Time)
			{
				ClearDotEffect(DamageType);
			}
		},
		TimeInterval, true, 0.01f);

}

void ADDEnemyBase::ApplyChainDamage(int DamageAmount, int NumberOfChain)
{
}

// Stun case : Time == DebuffRate
void ADDEnemyBase::ApplyDebuff(EDebuffType DebuffType, float Time, float DebuffRate)
{
	FDebuffState& DebuffState = DebuffStates[DebuffType];

	if (DebuffState.ElapsedTime > 0.0f && DebuffRate < DebuffState.AmountRate)
	{
		return;
	}

	DebuffState.ElapsedTime = 0.0f;
	DebuffState.AmountRate = DebuffRate;

	DebuffState.OnDebuffDelegate.Broadcast(DebuffState.TimerHandle, Time, DebuffState.AmountRate);
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

	OnDie.Broadcast(EnemyName, this);
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
	Stat->SetCurrentHp(MaxHP);
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

void ADDEnemyBase::ClearDotEffect(EDotDamageType DamageType)
{
	FDotEffectState* DotEffectState = DotEffectStates.Find(DamageType);
	if (DotEffectState)
	{
		GetWorld()->GetTimerManager().ClearTimer(DotEffectState->TimerHandle);
		DotEffectState->DamageAmount = 0.0f;
		DotEffectState->ElapsedTime = 0.0f;
	}
}

void ADDEnemyBase::ChangeMaxWalkSpeed(float Amount)
{
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed * Amount;
}

void ADDEnemyBase::Stun(FTimerHandle& TimerHandle, float Time, float Amount)
{
	// TODO : YSY High Cost, Need to Fix Algorithm.

	EnemyAIController->StopAI();
	UE_LOG(LogTemp, Warning, TEXT("StunStart"));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			EnemyAIController->RunAI();
			UE_LOG(LogTemp, Warning, TEXT("StunEnd"));
		}
	,0.01f, false, Time);
}
