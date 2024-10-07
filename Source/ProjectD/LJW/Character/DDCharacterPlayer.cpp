// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/Character/DDCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DDCharacterControlData.h"
#include "LJW/Weapon/DDWeaponSystemComponent.h"
#include "LJW/Animation/DDPlayerAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "LSM/BuildComponent/DDBuildComponent.h"
#include "YSY/Game/DDGameInstance.h"
#include "YSY/Manager/DDWaveManager.h"
#include "Blueprint/UserWidget.h"
#include "YSY/UI/DDPlayerHPBarWidget.h"
#include "LJW/CharacterStat/DDCharacterStatComponent.h"
#include "NiagaraComponent.h"
#include "LJW/Weapon/DDWeaponRifle.h"
#include "YSY/UI/DDMainWidget.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

ADDCharacterPlayer::ADDCharacterPlayer()
{
	//Control
	MouseSpeed = 1;

	//Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Data
	//static ConstructorHelpers::FObjectFinder<UDDCharacterControlData> DataRef(TEXT("/Script/Engine.Blueprint'/Game/0000/LJW/Blueprints/BP_DDCharacterControlData.BP_DDCharacterControlData'"));
	//if (DataRef.Object)
	//{
	//	CharacterControlManager = DataRef.Object;
	//}

	//Weapon System Component
	WeaponSystem = CreateDefaultSubobject<UDDWeaponSystemComponent>(TEXT("WeaponSystem"));

	//PlayerMode
	CurrentPlayerMode = EPlayerMode::CombatMode;

	//Collision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	//BuildComponent
	BuildSystem = CreateDefaultSubobject<UDDBuildComponent>(TEXT("BuildSystem"));

	//BuildHudNiagaraComponent
	BuildHudNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BuildHudNiagaraComponent"));
	BuildHudNiagaraComponent->SetupAttachment(RootComponent);
	BuildHudNiagaraComponent->SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HudNiagaraRef(TEXT("/Game/0000/LSM/VFX/NS_uiScriptor.NS_uiScriptor"));
	if (HudNiagaraRef.Succeeded())
	{
		BuildHudNiagaraComponent->SetAsset(HudNiagaraRef.Object);
	}

	BuildHudNiagaraComponent->SetRelativeRotation(FQuat(FRotator(0.0f, 90.0f, 0.f)));
	BuildHudNiagaraComponent->SetRelativeScale3D(FVector(7.0f));
	BuildHudNiagaraComponent->SetRelativeLocation(FVector(60.0f, 0.0f, 50.f));




	//StatComponent
	Stat = CreateDefaultSubobject<UDDCharacterStatComponent>(TEXT("Stat"));
#pragma region Init Input

	//Input
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_Jump.IA_Player_Jump'"));
	if (nullptr != JumpRef.Object)
	{
		JumpAction = JumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_Look.IA_Player_Look'"));
	if (nullptr != LookRef.Object)
	{
		LookAction = LookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_Move.IA_Player_Move'"));
	if (nullptr != MoveRef.Object)
	{
		MoveAction = MoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>BackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_BackMove.IA_Player_BackMove'"));
	if (nullptr != BackRef.Object)
	{
		BackMoveAction = BackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SprintRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_Sprint.IA_Player_Sprint'"));
	if (nullptr != SprintRef.Object)
	{
		SprintAction = SprintRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>MeleeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_EquipMelee.IA_Player_EquipMelee'"));
	if (nullptr != MeleeRef.Object)
	{
		EquipMeleeAction = MeleeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>RangeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_EquipRange.IA_Player_EquipRange'"));
	if (nullptr != RangeRef.Object)
	{
		EquipRangeAction = RangeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>SubSkillRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_ZoomSkill.IA_Player_ZoomSkill'"));
	if (nullptr != SubSkillRef.Object)
	{
		SubSkillAction = SubSkillRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>AttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_Attack.IA_Player_Attack'"));
	if (nullptr != AttackRef.Object)
	{
		AttackAction = AttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>EnterManagementModeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_EnterManagementMode.IA_Player_EnterManagementMode'"));
	if (nullptr != EnterManagementModeRef.Object)
	{
		EnterManagementModeAction = EnterManagementModeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>EnterBuildModeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_EnterBuildMode.IA_Player_EnterBuildMode'"));
	if (nullptr != EnterBuildModeRef.Object)
	{
		EnterBuildModeAction = EnterBuildModeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>WaveStartRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_WaveStart.IA_Player_WaveStart'"));
	if (nullptr != WaveStartRef.Object)
	{
		WaveStartAction = WaveStartRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>PlaceBuildingRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_PlaceBuilding.IA_Player_PlaceBuilding'"));
	if (nullptr != PlaceBuildingRef.Object)
	{
		PlaceBuildingAction = PlaceBuildingRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>CancleBuildModeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_CancleBuildMode.IA_Player_CancleBuildMode'"));
	if (nullptr != CancleBuildModeRef.Object)
	{
		CancleBuildModeAction = CancleBuildModeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>ReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/0000/LJW/Input/IA_Player_Reload.IA_Player_Reload'"));
	if (nullptr != ReloadRef.Object)
	{
		ReloadAction = ReloadRef.Object;
	}

	//Spawn And Die
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DieRef(TEXT("/Game/0000/LJW/Animation/AnimMontage/dead01_Montage.dead01_Montage"));
	if (nullptr != DieRef.Object)
	{
		DieMontage = DieRef.Object;
	}

#pragma endregion

}


void ADDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl();
	BindBuildingEvents();
	ResetPlayerState();

}

void ADDCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//AnimInstance Delegate Bind
	PlayerAnimInstance = Cast<UDDPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	//Montage Delegate Bind
	PlayerAnimInstance->OnMontageEnded.AddDynamic(this, &ADDCharacterPlayer::OnUnequipMontageEnded);

	WeaponSystem->OnSetAimingDelegate.BindUObject(PlayerAnimInstance, &UDDPlayerAnimInstance::SetIsAiming);
	WeaponSystem->OnGetAimingDelegate.BindUObject(PlayerAnimInstance, &UDDPlayerAnimInstance::GetIsAiming);
	WeaponSystem->OnSetWeaponIndexDelegate.BindUObject(PlayerAnimInstance, &UDDPlayerAnimInstance::SetWeaponIndex);

	Stat->OnHpZero.AddUObject(this, &ADDCharacterPlayer::Die);
}

void ADDCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//Jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(BackMoveAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::BackMoveTrue);
	EnhancedInputComponent->BindAction(BackMoveAction, ETriggerEvent::Canceled, this, &ADDCharacterPlayer::BackMoveFalse);
	EnhancedInputComponent->BindAction(BackMoveAction, ETriggerEvent::Completed, this, &ADDCharacterPlayer::BackMoveFalse);

	//Look
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::Look);

	//Sprint
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ADDCharacterPlayer::Walk);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADDCharacterPlayer::Walk);

	//Weapon System
	EnhancedInputComponent->BindAction(EquipMeleeAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::EquipMelee);
	EnhancedInputComponent->BindAction(EquipRangeAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::EquipRange);

	EnhancedInputComponent->BindAction(SubSkillAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::WeaponSubSkill);
	EnhancedInputComponent->BindAction(SubSkillAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::WeaponStartAiming);
	EnhancedInputComponent->BindAction(SubSkillAction, ETriggerEvent::Canceled, this, &ADDCharacterPlayer::WeaponEndAiming);
	EnhancedInputComponent->BindAction(SubSkillAction, ETriggerEvent::Completed, this, &ADDCharacterPlayer::WeaponEndAiming);

	//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ADDCharacterPlayer::WeaponAttack);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADDCharacterPlayer::WeaponAttack);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ADDCharacterPlayer::WeaponAttackEnd);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ADDCharacterPlayer::WeaponAttackEnd);

	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::WeaponReload);

	EnhancedInputComponent->BindAction(EnterManagementModeAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::EnterManagementMode);

	EnhancedInputComponent->BindAction(EnterBuildModeAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::OpenBuildWidget);

	EnhancedInputComponent->BindAction(WaveStartAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::WaveStart);

	EnhancedInputComponent->BindAction(PlaceBuildingAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::PlaceBuilding);

	EnhancedInputComponent->BindAction(CancleBuildModeAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::CancleBuildMode);
}

void ADDCharacterPlayer::SetCharacterControl()
{

	UDDCharacterControlData* NewPlayerControlData = CharacterControlManager;
	check(NewPlayerControlData);

	SetCharacterControlData(NewPlayerControlData);


	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewPlayerControlData->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

}

void ADDCharacterPlayer::SetCharacterControlData(const UDDCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SocketOffset = CharacterControlData->SocketOffset;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	MouseSpeed = CharacterControlData->MouseSpeed;
	SprintSpeed = CharacterControlData->MaxSprintSpeed;
	WalkSpeed = CharacterControlData->MaxWalkSpeed;
}

void ADDCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

}

void ADDCharacterPlayer::BackMoveTrue()
{
	bIsBackMove = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

void ADDCharacterPlayer::BackMoveFalse()
{
	bIsBackMove = false;
}

void ADDCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X * MouseSpeed);
	AddControllerPitchInput(LookAxisVector.Y * MouseSpeed);

	PlayerController->PlayerCameraManager->ViewPitchMin = -30.0f;
	PlayerController->PlayerCameraManager->ViewPitchMax = 30.0f;

}

void ADDCharacterPlayer::Sprint(const FInputActionValue& Value)
{

	if (PlayerAnimInstance->GetIsAiming() || bIsBackMove)
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ADDCharacterPlayer::Walk(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ADDCharacterPlayer::CreateLeaderPoseSkeletalMesh(TObjectPtr<USkeletalMeshComponent>& SkeletalMesh, const FString& Name, const FString& Path)
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(*Name);
	SkeletalMesh->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshRef(*Path);
	if (MeshRef.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(MeshRef.Object);
		SkeletalMesh->SetRelativeLocation(FVector(0, 0, 0));
		//Leader pose Component
		SkeletalMesh->SetLeaderPoseComponent(GetMesh());
	}
}


void ADDCharacterPlayer::OnUnequipMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (WeaponSystem->IsUnequipMontage(Montage))
	{
		WeaponSystem->PlayEquipMontage();
	}
}

void ADDCharacterPlayer::SetCameraFOV(const float& Amount)
{
	FollowCamera->SetFieldOfView(Amount);
}

UDDWeaponSystemComponent* ADDCharacterPlayer::GetWeaponComp()
{
	return WeaponSystem;
}


void ADDCharacterPlayer::EquipMelee()
{
	if (!(PlayerAnimInstance->IsAnyMontagePlaying()))
	{
		OnVisibilityAmmoTextChanged.Broadcast(false);
		OnVisibilityWeaponSlotChanged.Broadcast(1);
		WeaponSystem->EquipMeleeWeapon();
		CurrentPlayerMode = EPlayerMode::CombatMode;
		SetPlayerGameMode();
		BuildSystem->AllStopTrace();
		BuildHudNiagaraComponent->SetActive(false);
	}
}


void ADDCharacterPlayer::EquipRange()
{

	if (!(PlayerAnimInstance->IsAnyMontagePlaying()))
	{
		OnVisibilityAmmoTextChanged.Broadcast(true);
		OnVisibilityWeaponSlotChanged.Broadcast(2);
		WeaponSystem->EquipRangeWeapon();
		CurrentPlayerMode = EPlayerMode::CombatMode;
		SetPlayerGameMode();
		BuildSystem->AllStopTrace();
		BuildHudNiagaraComponent->SetActive(false);
	}
}

void ADDCharacterPlayer::WeaponSubSkill()
{
	//Enum
	if (CurrentPlayerMode == EPlayerMode::CombatMode)
	{
		WeaponSystem->WeaponSubSkill();

	}
}

void ADDCharacterPlayer::WeaponStartAiming()
{
	//Enum
	if (CurrentPlayerMode == EPlayerMode::CombatMode)
	{
		WeaponSystem->WeaponStartAiming();
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

}

void ADDCharacterPlayer::WeaponEndAiming()
{
	//Enum
	if (CurrentPlayerMode == EPlayerMode::CombatMode)
	{
		WeaponSystem->WeaponEndAiming();
	}
}

void ADDCharacterPlayer::WeaponAttack()
{
	//Enum
	if (CurrentPlayerMode == EPlayerMode::CombatMode)
	{
		WeaponSystem->WeaponAttack();
	}
}

void ADDCharacterPlayer::WeaponAttackEnd()
{
	if (CurrentPlayerMode == EPlayerMode::CombatMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackEnd"));
		WeaponSystem->ResetWeaponSound();
	}
}

void ADDCharacterPlayer::WeaponReload()
{
	if (CurrentPlayerMode == EPlayerMode::CombatMode)
	{
		WeaponSystem->ReloadWeapon();
	}
}

void ADDCharacterPlayer::AddAggro()
{
	CurrentAggroNum++;
	CurrentAggroNum = std::min(CurrentAggroNum, MaxAggroNum);
}

void ADDCharacterPlayer::SubtractAggro()
{
	CurrentAggroNum--;
	CurrentAggroNum = std::max(CurrentAggroNum, 0);
}

bool ADDCharacterPlayer::IsMaxAggro()
{
	return CurrentAggroNum >= MaxAggroNum;
}

void ADDCharacterPlayer::EnterManagementMode()
{
	if (CurrentPlayerMode == EPlayerMode::BuildMode || PlayerAnimInstance->IsAnyMontagePlaying())
	{
		return;
	}
	BuildHudNiagaraComponent->SetActive(true);
	OnVisibilityWeaponSlotChanged.Broadcast(3);
	CurrentPlayerMode = EPlayerMode::ManagementMode;
	BuildSystem->StartManageTrace();
}

void ADDCharacterPlayer::OpenBuildWidget()
{
	if (CurrentPlayerMode == EPlayerMode::ManagementMode)
	{
		BuildSystem->ShowStartBuildWidget();
		SetPlayerMoveOnlyMode();
	}
}

void ADDCharacterPlayer::BuildTrapOrTower()
{
	FInputModeGameOnly InputModeGameOnlyData;

	CurrentPlayerMode = EPlayerMode::BuildMode;
}

void ADDCharacterPlayer::PlaceBuilding()
{
	if (CurrentPlayerMode == EPlayerMode::BuildMode)
	{
		if (BuildSystem->PlaceBuilding())
		{
			/*CurrentPlayerMode = EPlayerMode::ManagementMode;
			BuildSystem->AllStopTrace();
			BuildSystem->StartManageTrace();*/
		}
	}
	else if (CurrentPlayerMode == EPlayerMode::ManagementMode)
	{
		//TODO : YSY or LJW Upgrade Widget

		FName BuildingName = BuildSystem->GetManagedBuildingRowName();
		if (BuildingName == NAME_None)
		{
			return;
		}

		SetPlayerCanNotMoveMode();
		UE_LOG(LogTemp, Warning, TEXT("Check Here"));
		BuildSystem->ShowUpgradeBuildingWidget();
	}
}

void ADDCharacterPlayer::CancleBuildMode()
{
	if (CurrentPlayerMode == EPlayerMode::BuildMode)
	{
		BuildSystem->CancelReadyBuilding();
		CurrentPlayerMode = EPlayerMode::ManagementMode;
	}
}

void ADDCharacterPlayer::BindBuildingEvents()
{
	BuildSystem->OnStartBuild.AddDynamic(this, &ADDCharacterPlayer::BuildTrapOrTower);
}

void ADDCharacterPlayer::WaveStart()
{
	auto GameInstance = Cast<UDDGameInstance>(GetGameInstance());
	GameInstance->GetWaveManager()->WaveStart();
}

void ADDCharacterPlayer::SetPlayerCanNotMoveMode()
{
	FInputModeUIOnly InputModeUIOnly;
	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->SetIgnoreLookInput(true);
	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->SetShowMouseCursor(true);
}

void ADDCharacterPlayer::SetPlayerMoveOnlyMode()
{
	FInputModeGameAndUI InputModeGameAndUIData;
	PlayerController->SetIgnoreMoveInput(false);
	PlayerController->SetIgnoreLookInput(false);
	PlayerController->SetInputMode(InputModeGameAndUIData);
	PlayerController->SetShowMouseCursor(true); // 마우스 커서 표시
}

void ADDCharacterPlayer::SetPlayerGameMode()
{
	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetIgnoreMoveInput(false);
	PlayerController->SetIgnoreLookInput(false);
	PlayerController->SetInputMode(InputModeGameOnly);
	PlayerController->SetShowMouseCursor(false); // 마우스 커서 표시

}

void ADDCharacterPlayer::Spawn()
{
	FindSpawnPoint();
	// 캐릭터를 다시 활성화
	SetActorHiddenInGame(false);

	WeaponAttackEnd();
	WeaponEndAiming();

	Stat->SetHp(PlayerMaxHp);
	this->SetActorLocation(SpawnLocation);
	IsDie = false;
	// 입력 활성화
	EnableInput(Cast<APlayerController>(GetController()));
}

void ADDCharacterPlayer::Die()
{
	// 입력 비활성화
	DisableInput(Cast<APlayerController>(GetController()));
	IsDie = true;
	WeaponAttackEnd();
	WeaponEndAiming();

	if (PlayerAnimInstance && DieMontage)
	{
		// 몽타주 재생 및 종료 델리게이트 설정
		PlayerAnimInstance->Montage_Play(DieMontage);
		if (!PlayerAnimInstance->OnMontageEnded.IsAlreadyBound(this, &ADDCharacterPlayer::OnDieMontageEnded))
		{
			PlayerAnimInstance->OnMontageEnded.AddDynamic(this, &ADDCharacterPlayer::OnDieMontageEnded);
		}
	}
}

void ADDCharacterPlayer::FindSpawnPoint()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundActors);

	bool bSpawnPointFound = false;

	for (AActor* Actor : FoundActors)
	{
		if (Actor->ActorHasTag("Spawn"))
		{
			SpawnLocation = Actor->GetActorLocation();
			bSpawnPointFound = true;
			break; // 첫 번째로 찾은 스폰 포인트 사용
		}
	}

	if (!bSpawnPointFound)
	{
		SpawnLocation = this->GetActorLocation() + FVector(0, 0, 300.f);
		UE_LOG(LogTemp, Warning, TEXT("No spawn point found with tag 'Spawn'"));
	}
}

void ADDCharacterPlayer::OnDieMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DieMontage)
	{
		// 델리게이트 해제 (중복 호출 방지)
		PlayerAnimInstance->OnMontageEnded.RemoveDynamic(this, &ADDCharacterPlayer::OnDieMontageEnded);
		
		// 캐릭터를 비활성화 (보이지 않도록)
		SetActorHiddenInGame(true);

		Spawn();

		// 2초 후에 Spawn 함수를 호출하도록 타이머 설정
		//GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ADDCharacterPlayer::Spawn, 2.0f, false);


		if (!bInterrupted)
		{
			// 방해받았을 경우
		}
	}
}

void ADDCharacterPlayer::ResetPlayerState()
{
	Stat->SetHp(PlayerMaxHp);
	CurrentAggroNum = 0;
	EquipMelee();
	WeaponSystem->ResetWeaponState();
}

void ADDCharacterPlayer::SetupCharacterWidget(UDDUserWidget* InUserWidget)
{
	UDDPlayerHPBarWidget* HpBarWidget = Cast<UDDPlayerHPBarWidget>(InUserWidget);
	ensure(HpBarWidget);
	if (HpBarWidget)
	{
		// TODO : YSY or LJW Remove MagicNumber. Need to StatComponent
		//Stat->SetHp(100.0f);
		HpBarWidget->UpdateStat(Stat->GetCurrentHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UDDPlayerHPBarWidget::UpdateHpBar);
	}
}

void ADDCharacterPlayer::SetupWeaponWidget(UDDUserWidget* InUserWidget)
{
	ADDWeaponRifle* WeaponRifle = Cast<ADDWeaponRifle>(WeaponSystem->GetCurrentRangeWeaponInstance());
	if (WeaponRifle && InUserWidget)
	{
		UDDMainWidget* MainWidget = Cast<UDDMainWidget>(InUserWidget);
		if (MainWidget)
		{
			MyMainWidget = MainWidget;
			// WeaponRifle->OnLoadedAmmoChanged에 바인딩된 함수가 있는지 확인
			if (!WeaponRifle->OnLoadedAmmoChanged.IsAlreadyBound(MainWidget, &UDDMainWidget::SetLoadedRifleAmmoText))
			{
				WeaponRifle->OnLoadedAmmoChanged.AddDynamic(MainWidget, &UDDMainWidget::SetLoadedRifleAmmoText);
			}

			// WeaponRifle->OnUnLoadedAmmoChanged에 바인딩된 함수가 있는지 확인
			if (!WeaponRifle->OnUnLoadedAmmoChanged.IsAlreadyBound(MainWidget, &UDDMainWidget::SetUnLoadedRifleAmmoText))
			{
				WeaponRifle->OnUnLoadedAmmoChanged.AddDynamic(MainWidget, &UDDMainWidget::SetUnLoadedRifleAmmoText);
			}
			MainWidget->SetRifleAmmoText(WeaponRifle->GetLoadedAmmo(), WeaponRifle->GetUnloadedAmmo());
			MainWidget->SetVisibilityAmmoText(false);
			// OnVisibilityAmmoTextChanged에 바인딩된 함수가 있는지 확인
			if (!OnVisibilityAmmoTextChanged.IsAlreadyBound(MainWidget, &UDDMainWidget::SetVisibilityAmmoText))
			{
				OnVisibilityAmmoTextChanged.AddDynamic(MainWidget, &UDDMainWidget::SetVisibilityAmmoText);
			}

			// OnVisibilityWeaponSlotChanged에 바인딩된 함수가 있는지 확인
			if (!OnVisibilityWeaponSlotChanged.IsAlreadyBound(MainWidget, &UDDMainWidget::SetWeaponSlotActive))
			{
				OnVisibilityWeaponSlotChanged.AddDynamic(MainWidget, &UDDMainWidget::SetWeaponSlotActive);
			}
		}
	}
}

void ADDCharacterPlayer::PopulateMaterials(TObjectPtr<USkeletalMeshComponent> MeshComponent, TArray<TObjectPtr<UMaterialInterface>>& MaterialArray)
{
	if (MeshComponent)
	{
		for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
		{
			MaterialArray.Add(MeshComponent->GetMaterial(i));
		}
	}
}

void ADDCharacterPlayer::SetMaterialForMesh(TObjectPtr<USkeletalMeshComponent> MeshComponent, TObjectPtr<UMaterialInterface> Material)
{
	if (MeshComponent && Material)
	{
		for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
		{
			MeshComponent->SetMaterial(i, Material);
		}
	}
}

void ADDCharacterPlayer::RestoreMaterialsForMesh(TObjectPtr<USkeletalMeshComponent> MeshComponent, const TArray<TObjectPtr<UMaterialInterface>>& OriginalMaterials)
{
	if (MeshComponent)
	{
		for (int32 i = 0; i < OriginalMaterials.Num(); ++i)
		{
			MeshComponent->SetMaterial(i, OriginalMaterials[i]);
		}
	}
}

void ADDCharacterPlayer::DamageInterval()
{
	CanBeDamaged = true;
}

void ADDCharacterPlayer::ApplyHitMaterial()
{
	CanBeDamaged = false;
}

void ADDCharacterPlayer::RestoreOriginalMaterials()
{
}

float ADDCharacterPlayer::ApplyDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!CanBeDamaged || IsDie)
	{
		return -1.f;
	}
	UE_LOG(LogTemp, Warning, TEXT("Character Damaged"));
	Stat->ApplyDamage(DamageAmount);
	ApplyHitMaterial();
	// 1초 후 원래 메터리얼 복구
	GetWorldTimerManager().SetTimer(
		DamageTimer,
		this,
		&ADDCharacterPlayer::DamageInterval,
		0.5f, // 복구 지연 시간
		false // 반복하지 않음
	);

	GetWorldTimerManager().SetTimer(
		RestoreOriginalMateirlTimer,
		this,
		&ADDCharacterPlayer::RestoreOriginalMaterials,
		0.2f, // 복구 지연 시간
		false // 반복하지 않음
	);

	return DamageAmount;
}

void ADDCharacterPlayer::ApplyDamageOverTime(EDotDamageType DamageType, float Time, float TimeInterval, float DamageAmount)
{
}

void ADDCharacterPlayer::ApplyChainDamage(int DamageAmount, int NumberOfChain)
{
}

void ADDCharacterPlayer::ApplyDebuff(EDebuffType DebuffType, float Time, float DebuffRate)
{
}

void ADDCharacterPlayer::GetAmmo()
{
	WeaponSystem->GetAmmo();
}



