// Fill out your copyright notice in the Description page of Project Settings.


#include "YSY/Enemy/DDEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "YSY/AI/DDEnemyAIController.h"
#include "YSY/AI/AISplineRoute.h"
#include "YSY/UI/DDHpBarWidget.h"
#include "YSY/UI/DDWidgetComponent.h"
#include "YSY/Stat/DDEnemyStatComponent.h"
#include "YSY/DamageType/AllDamageType.h"
#include "YSY/Animation/AttackFinishedAnimNotify.h"
#include "YSY/Animation/AttackTraceAnimNotify.h"
#include "YSY/Animation/DDPlayEffectAnimNotify.h"
#include "YSY/Interface/AggroTargetInterface.h"
#include "YSY/Collision/CollisionChannel.h"
#include "YSY/Game/DDPlayerState.h"
#include "Components/AudioComponent.h"
#include "YSY/Interface/AmmoInterface.h"

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
	
	AttackAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSound"));
	AttackAudio->SetupAttachment(GetMesh());
	AttackAudio->bAutoActivate = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/0000/YSY/Widget/YSY_WBP_EnemyHpBar.YSY_WBP_EnemyHpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(GTCHANNEL_PLAYER, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(GTCHANNEL_ENEMY, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 300.0f;
	GetCharacterMovement()->bConstrainToPlane = true;

	GetCharacterMovement()->bEnablePhysicsInteraction = false;
	GetCharacterMovement()->bPushForceUsingZOffset = true;
	GetCharacterMovement()->bPushForceScaledToMass = true;
	GetCharacterMovement()->PushForceFactor = 0.0f;
	SetActorEnableCollision(false);
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

	ShowHpBarbyAttack();
	
	const UDamageType* DamageType = DamageEvent.DamageTypeClass.GetDefaultObject();

	float ActualDamage = DamageAmount;
	
	if (DamageType)
	{
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
	}


	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *(PointDamageEvent->HitInfo.BoneName.ToString()));

		for (const FName& Weakpoint : WeakPoints)
		{
			if (PointDamageEvent->HitInfo.BoneName == Weakpoint)
			{
				ActualDamage *= 1.5f; // TODO : YSY WeakPoint Caculation
				//UE_LOG(LogTemp, Warning, TEXT("WeakPoint"));
			}
		}
	}

	Stat->ApplyStatDamage(ActualDamage * Stat->GetDamageReceiveRate());
	
	return ActualDamage;
}

void ADDEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<ADDEnemyAIController>(GetController());
	check(EnemyAIController);

	EnemyAIController->OnMoveFinished.BindUObject(this, &ADDEnemyBase::SplineMoveFinish);

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	OnSetVisibleHpBarDelegate.ExecuteIfBound(false);
}

void ADDEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdateWidgetScale(); // TODO : YSY Doesn't work. Move to SetTimerEvent

	if (bIsAggroState && bIsCanTurn)
	{
		auto RotateTemp = (Player->GetActorLocation() - GetActorLocation()).Rotation();
		RotateTemp.Pitch = 0;

		SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotateTemp, DeltaTime, TurnSpeed));
	}
}

void ADDEnemyBase::InitializeEnemy(const FDDEnemyData& EnemyData)
{
	EnemyName = EnemyData.EnemyName;
	WeakPoints = EnemyData.WeakPoints;
	EnemyType = EnemyData.EnemyType;
	EnemyAttackType = EnemyData.EnemyAttackType;
	MaxHP = EnemyData.MaxHP;
	Stat->SetMaxHp(MaxHP);
	Stat->SetCurrentHp(MaxHP);
	MovementSpeed = EnemyData.MovementSpeed;
	ChangeMaxWalkSpeed(1.0f);
	AttackSpeed = EnemyData.AttackSpeed;
	Damage = EnemyData.Damage;
	AttackRange = EnemyData.AttackRange;
	AggroRange = EnemyData.AggroRange;
	HpBar->SetRelativeLocation({ 0.0f,0.0f,EnemyData.HealthWidgetHeight });
	GoldDropAmount = EnemyData.GoldDropAmount;
	bIsBoss = EnemyData.bIsBoss;
	bIsElite = EnemyData.bIsElite;
	AttackEffects = EnemyData.AttackEffects;
	AttackAudio->SetSound(EnemyData.AttackSound);
	AttackAudio->SetVolumeMultiplier(EnemyData.VolumeMultiplier);
	AttackAudio->SetPitchMultiplier(EnemyData.PitchMultiplier);
	SoundStartTime = EnemyData.SoundStartTime;
	DeathEffects = EnemyData.DeathEffects;
	GetCapsuleComponent()->SetCapsuleRadius(EnemyData.CapsuleRadiusSize);
	GetCapsuleComponent()->SetCapsuleHalfHeight(EnemyData.CapsuleHalfHeightSize);

	float EnemyScale = EnemyData.ScaleSize;
	SetActorScale3D({ EnemyScale, EnemyScale, EnemyScale });

	if (!EnemyData.SkeletalMesh.IsValid())
	{
		EnemyData.SkeletalMesh.LoadSynchronous();
	}

	USkeletalMesh* EnemySkeletalMesh = EnemyData.SkeletalMesh.Get();
	GetMesh()->SetSkeletalMesh(EnemySkeletalMesh);
	GetMesh()->SetRelativeLocationAndRotation({ 0,0,EnemyData.MeshLocationZ }, { 0,-90,0 });
	GetMesh()->SetCollisionProfileName(FName("Enemy"));
	GetMesh()->SetGenerateOverlapEvents(true);

	/*if (!EnemyData.AnimationBlueprint.IsValid())
	{
		EnemyData.AnimationBlueprint.LoadSynchronous();
	}*/

	//// 유효성 검사
	//if (!EnemyData.AnimationBlueprint.IsValid())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed to load Animation Blueprint class - Check1"));
	//}

	//if (!EnemyData.AnimationBlueprint.Get())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve Animation Blueprint class - Check2"));
	//}
	//else
	//{
	//	// TSoftClassPtr<UAnimInstance>를 사용하여 애니메이션 인스턴스 클래스를 로드합니다.
	//	TSubclassOf<UAnimInstance> AnimBPClass = EnemyData.AnimationBlueprint.Get();

	//	if (AnimBPClass)
	//	{
	//		// 애니메이션 인스턴스 클래스를 메쉬에 설정합니다.
	//		GetMesh()->SetAnimInstanceClass(AnimBPClass);
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("AnimBPClass is null - Check3"));
	//	}
	//}

	// 원래 코드
	//UAnimBlueprint* AnimBP = EnemyData.AnimationBlueprint.Get();
	//GetMesh()->SetAnimInstanceClass(AnimBP->GeneratedClass);

	UClass* AnimInstanceClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, *EnemyData.AnimationBlueprint);

	if (AnimInstanceClass)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	}

	if (!EnemyData.AttackMontage.IsValid())
	{
		EnemyData.AttackMontage.LoadSynchronous();
	}

	UAnimMontage* EnemyAttackMontage = EnemyData.AttackMontage.Get();
	AttackMontage = DuplicateObject<UAnimMontage>(EnemyAttackMontage, this);

	BindingAnimNotify();


}

void ADDEnemyBase::SplineMove()
{
	if (bIsDead)
	{
		return;
	}
	if (AIMoveRoute)
	{
		FVector Destination = AIMoveRoute->GetSplinePointasWorldPosition(RouteIndex);
		RouteIndex++;
		EnemyAIController->MoveToLocation(Destination);
	}
}

void ADDEnemyBase::SetAIMoveFinishedDelegate(const FAISplineMoveOnFinishedSignature& InOnSplineMoveFinished)
{
	OnSplineMoveFinished = InOnSplineMoveFinished;
}

void ADDEnemyBase::AttackByAI()
{
	// TODO : YSY Complete Attack
	ChangeMaxWalkSpeed(0.0f);
	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);

	if (EnemyAttackType == EEnemyAttackType::Range)
	{
		bIsCanTurn = false;
	}
	
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

float ADDEnemyBase::GetAITurnSpeed() const noexcept
{
	return TurnSpeed;
}

void ADDEnemyBase::SetupCharacterWidget(UDDUserWidget* InUserWidget)
{
	UDDHpBarWidget* HpBarWidget = Cast<UDDHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetMaxHp());
		HpBarWidget->SetOwnerName(EnemyName);
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UDDHpBarWidget::UpdateHpBar);

		OnSetVisibleHpBarDelegate.BindUObject(HpBarWidget, &UDDHpBarWidget::SetVisiblePorgressBar);
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

	if (AIMoveRoute && AIMoveRoute->IsSplineEnd(RouteIndex))
	{
		ArrivalAtGoal();
	}
}

void ADDEnemyBase::ArrivalAtGoal()
{
	/*GetMesh()->SetVisibility(false, true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	//EnemyAIController->StopAI();
	OnDie.Broadcast(EnemyName, this);

	// TODO : YSY Caculate Potal Count, Remove MagicNum

	if (OnSubRemainingLivesSignature.IsBound())
	{
		OnSubRemainingLivesSignature.Execute(1);
	}

}

void ADDEnemyBase::Die()
{
	// TODO : YSY Player get gold, Drop Item
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;
	AttackAudio->Stop();
	for (const auto& Elem : NiagaraAttackEffect)
	{
		if (Elem)
		{
			Elem->DestroyComponent();
		}
	}
	NiagaraAttackEffect.Empty();
	PlayDeathEffect();
	OnSetVisibleHpBarDelegate.ExecuteIfBound(false);
	OnDie.Broadcast(EnemyName, this);
	ADDPlayerState* DDPlayerState = Cast<ADDPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	DDPlayerState->AddGold(GoldDropAmount);

	auto PlayerAmmoInterface = Cast<IAmmoInterface>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerAmmoInterface->GetAmmo();
	//EnemyAIController->StopAI();
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
	ensure(Stat);
	if (!Stat)
	{
		return;
	}
	EnemyAIController->StopMovement();
	Stat->SetCurrentHp(MaxHP);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	GetMesh()->SetVisibility(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorLocation(AIMoveRoute->GetSplinePointasWorldPosition(0));
	RouteIndex = 0;
	EnemyAIController->RunAI();
	bIsDead = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	SetActorEnableCollision(true);
	GetCharacterMovement()->GravityScale = 1.0f;
	GetMesh()->SetCollisionObjectType(GTCHANNEL_TOWER);
}

void ADDEnemyBase::Deactivate()
{
	if (AIMoveRoute)
	{
		SetActorLocation({ AIMoveRoute->GetSplinePointasWorldPosition(0) + FVector(0,0,500.0f) });
		EnemyAIController->StopMovement();
	}
	//GetMesh()->GetAnimInstance()->Montage_Stop(0.01f);
	AttackFinished();
	if (bIsAggroState)
	{
		IAggroTargetInterface* AggroTargetInterface = Cast<IAggroTargetInterface>(Player);
		AggroTargetInterface->SubtractAggro();
	}
	bIsAggroState = false;
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	EnemyAIController->StopAI();
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	RouteIndex = 0;
	SetActorEnableCollision(false);
	GetCharacterMovement()->GravityScale = 0.0f;
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

void ADDEnemyBase::BindingAnimNotify()
{
	if (AttackMontage)
	{
		TArray<FAnimNotifyEvent> notifyEvents{ AttackMontage->Notifies };

		for (FAnimNotifyEvent eventNotify : notifyEvents)
		{
			if (UAttackFinishedAnimNotify* AttackFinishedNotify = Cast<UAttackFinishedAnimNotify>(eventNotify.Notify))
			{
				AttackFinishedNotify->OnNotified.AddUObject(this, &ADDEnemyBase::AttackFinished);
			}
			else if (UAttackTraceAnimNotify* AttackTraceNotify = Cast<UAttackTraceAnimNotify>(eventNotify.Notify))
			{
				if (EnemyAttackType == EEnemyAttackType::Melee)
				{
					AttackTraceNotify->OnNotified.AddUObject(this, &ADDEnemyBase::MeleeAttack);
				}
				else if(EnemyAttackType == EEnemyAttackType::Range)
				{
					AttackTraceNotify->OnNotified.AddUObject(this, &ADDEnemyBase::RangeAttack);
				}			
			}
			else if (UDDPlayEffectAnimNotify* PlayEffectNotify = Cast<UDDPlayEffectAnimNotify>(eventNotify.Notify))
			{
				PlayEffectNotify->OnNotified.AddUObject(this, &ADDEnemyBase::PlayAttackEffect);
			}
		}
	}
}

void ADDEnemyBase::AttackFinished()
{
	ChangeMaxWalkSpeed(1.0f);
	bIsCanTurn = true;
	OnAttackFinished.ExecuteIfBound();
}

void ADDEnemyBase::MeleeAttack()
{
	TArray<AActor*> IgnoreActors;
	FHitResult OutHit;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(), AttackRange, ETraceTypeQuery::TraceTypeQuery7, false, IgnoreActors, EDrawDebugTrace::None, OutHit, true);
	// TODO : YSY Change TraceTypeQuery to name

	if (bHit)
	{
		if (auto HitActor = Cast<IDamageInterface>(OutHit.GetActor()))
		{
			FDamageEvent DamageEvent;
			HitActor->ApplyDamage(Damage, DamageEvent, EnemyAIController, this);
		}
	}
}

void ADDEnemyBase::RangeAttack()
{
	TArray<AActor*> IgnoreActors;
	FHitResult OutHit;

	FVector Start = GetActorLocation();
	FVector End = GetActorForwardVector() * 10000.0 + GetActorLocation(); // TODO : YSY MagicNumber, DataTable
	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery7, false, IgnoreActors, EDrawDebugTrace::None, OutHit, true);
	// TODO : YSY Change TraceTypeQuery to name

	if (bHit)
	{
		if (auto HitActor = Cast<IDamageInterface>(OutHit.GetActor()))
		{
			FDamageEvent DamageEvent;
			HitActor->ApplyDamage(Damage, DamageEvent, EnemyAIController, this);
		}
	}
}

void ADDEnemyBase::ShowHpBarbyAttack()
{
	SetVisibleHpBar(true);

	GetWorld()->GetTimerManager().SetTimer(HpBarTH, [this]()
		{
			SetVisibleHpBar(false);
		}, 0.1f, false, 4.0f);
}

void ADDEnemyBase::PlayAttackEffect()
{
	AttackAudio->Play(SoundStartTime);
	NiagaraAttackEffect.Empty();

	for (const auto& AttackEffectInfo : AttackEffects)
	{
		for (const auto& LocationName : AttackEffectInfo.LocationNames)
		{
			auto Location = GetMesh()->GetSocketLocation(LocationName);
			auto Rotation = GetMesh()->GetSocketRotation(LocationName);

			if (AttackEffectInfo.CascadeEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackEffectInfo.CascadeEffect, Location, Rotation, FVector(AttackEffectInfo.EffectScale));
			}

			if (AttackEffectInfo.NiagaraEffect)
			{
				auto TempEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackEffectInfo.NiagaraEffect, Location, Rotation, FVector(AttackEffectInfo.EffectScale));
				
				NiagaraAttackEffect.Add(TempEffect);
			}
		}
	}
}

void ADDEnemyBase::PlayDeathEffect()
{
	for (const auto& DeathEffectInfo : DeathEffects)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathEffectInfo.SoundEffect, GetActorLocation(), DeathEffectInfo.VolumeMultiplier, DeathEffectInfo.PitchMultiplier, DeathEffectInfo.SoundStartTime);

		for (const auto& LocationName : DeathEffectInfo.LocationNames)
		{
			auto Location = GetMesh()->GetSocketLocation(LocationName);
			auto Rotation = GetMesh()->GetSocketRotation(LocationName);

			if (DeathEffectInfo.CascadeEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffectInfo.CascadeEffect, Location, Rotation, FVector(DeathEffectInfo.EffectScale));
			}

			if (DeathEffectInfo.NiagaraEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffectInfo.NiagaraEffect, Location, Rotation, FVector(DeathEffectInfo.EffectScale));
			}
		}
	}
}

void ADDEnemyBase::SetVisibleHpBar(bool bIsVisible)
{
	OnSetVisibleHpBarDelegate.ExecuteIfBound(bIsVisible);
}
