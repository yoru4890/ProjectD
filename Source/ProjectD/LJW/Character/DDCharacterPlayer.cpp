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
	static ConstructorHelpers::FObjectFinder<UDDCharacterControlData> DataRef(TEXT("/Script/Engine.Blueprint'/Game/0000/LJW/Blueprints/BP_DDCharacterControlData.BP_DDCharacterControlData'"));
	if (DataRef.Object)
	{
		CharacterControlManager = DataRef.Object;
	}
	
	//Weapon System Component
	WeaponSystem = CreateDefaultSubobject<UDDWeaponSystemComponent>(TEXT("WeaponSystem"));
	
	//PlayerMode
	CurrentPlayerMode = EPlayerMode::CombatMode; 

	//Collision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	//BuildComponent
	BuildSystem = CreateDefaultSubobject<UDDBuildComponent>(TEXT("BuildSystem"));

	//BuildWidget
	static ConstructorHelpers::FClassFinder<UUserWidget> BuildWidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/YSY/Widget/YSY_WBP_RM_Select.YSY_WBP_RM_Select_C'"));

	if (BuildWidgetFinder.Succeeded())
	{
		BuildWidgetClass = BuildWidgetFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> RMMachineGunWidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/YSY/Widget/YSY_WBP_RM_MachineGun.YSY_WBP_RM_MachineGun_C'"));

	if (RMMachineGunWidgetFinder.Succeeded())
	{
		RMMachineGunWidgetClass = RMMachineGunWidgetFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UpMachineGunWidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/0000/YSY/Widget/YSY_WBP_RM_UpMachineGun.YSY_WBP_RM_UpMachineGun_C'"));

	if (UpMachineGunWidgetFinder.Succeeded())
	{
		UpMachineGunWidgetClass = UpMachineGunWidgetFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> RMThornTrapWidgetFinder(TEXT(""));

	if (RMThornTrapWidgetFinder.Succeeded())
	{
		RMThornTrapWidgetClass = RMThornTrapWidgetFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UpThornTrapWidgetFinder(TEXT(""));

	if (UpThornTrapWidgetFinder.Succeeded())
	{
		UpThornTrapWidgetClass = UpThornTrapWidgetFinder.Class;
	}

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
	InitWidget();
	
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

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ADDCharacterPlayer::WeaponAttack);

	EnhancedInputComponent->BindAction(EnterManagementModeAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::EnterManagementMode);
	
	EnhancedInputComponent->BindAction(EnterBuildModeAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::OpenBuildWidget);

	EnhancedInputComponent->BindAction(WaveStartAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::WaveStart);

	EnhancedInputComponent->BindAction(PlaceBuildingAction, ETriggerEvent::Started, this, &ADDCharacterPlayer::PlaceBuilding);
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

void ADDCharacterPlayer::CreateLeaderPoseSkeletalMesh(USkeletalMeshComponent* SkeletalMesh, const FString& Name, const FString& Path)
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
		WeaponSystem->EquipMeleeWeapon();
		CurrentPlayerMode = EPlayerMode::CombatMode;
		BuildSystem->AllStopTrace();
	}
}


void ADDCharacterPlayer::EquipRange()
{

	if (!(PlayerAnimInstance->IsAnyMontagePlaying()))
	{
		WeaponSystem->EquipRangeWeapon();
		CurrentPlayerMode = EPlayerMode::CombatMode;
		BuildSystem->AllStopTrace();
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
	CurrentPlayerMode = EPlayerMode::ManagementMode;
	BuildSystem->StartManageTrace();
}

void ADDCharacterPlayer::OpenBuildWidget()
{
	if (CurrentPlayerMode == EPlayerMode::ManagementMode)
	{
		BuildWidget->AddToViewport();
		SetPlayerUIMode();
	}
}

void ADDCharacterPlayer::BuildTrapOrTower(const FName& BuildingName)
{
	FInputModeGameOnly InputModeGameOnlyData;

	CurrentPlayerMode = EPlayerMode::BuildMode;
	BuildSystem->AllStopTrace();
	BuildSystem->StartBuildTrace();
	BuildSystem->ReadyBuilding(BuildingName);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(InputModeGameOnlyData);
	PlayerController->SetIgnoreMoveInput(false);
	BuildWidget->SetVisibility(ESlateVisibility::Visible);
	BuildWidget->RemoveFromParent();
}

void ADDCharacterPlayer::PlaceBuilding()
{
	if (CurrentPlayerMode == EPlayerMode::BuildMode)
	{
		if (BuildSystem->PlaceBuilding())
		{
			CurrentPlayerMode = EPlayerMode::ManagementMode;
			BuildSystem->AllStopTrace();
			BuildSystem->StartManageTrace();
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
		else if (BuildingName == FName("MachineGunTower"))
		{
			RMMachineGunWidget->AddToViewport();
			SetPlayerUIMode();
		}
		else if (BuildingName == FName("UpgradeMachineGunTower"))
		{
			UpMachineGunWidget->AddToViewport();
			SetPlayerUIMode();
		}
		else if (BuildingName == FName("ThornTrap"))
		{
			RMThornTrapWidget->AddToViewport();
			SetPlayerUIMode();
		}
		else if (BuildingName == FName("UpgradeThornTrap"))
		{
			UpThornTrapWidget->AddToViewport();
			SetPlayerUIMode();
		}

		
	}
}

void ADDCharacterPlayer::WaveStart()
{
	auto GameInstance = Cast<UDDGameInstance>(GetGameInstance());
	GameInstance->GetWaveManager()->WaveStart();
}

void ADDCharacterPlayer::InitWidget()
{
	BuildWidget = CreateWidget(GetWorld(), BuildWidgetClass);
	RMMachineGunWidget = CreateWidget(GetWorld(), RMMachineGunWidgetClass);
	UpMachineGunWidget = CreateWidget(GetWorld(), UpMachineGunWidgetClass);
	RMThornTrapWidget = CreateWidget(GetWorld(), RMThornTrapWidgetClass);
	UpThornTrapWidget = CreateWidget(GetWorld(), UpThornTrapWidgetClass);
}

void ADDCharacterPlayer::SetPlayerUIMode()
{
	FInputModeUIOnly InputModeUIOnlyData;
	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->StopMovement();
	PlayerController->SetInputMode(InputModeUIOnlyData);
	PlayerController->SetShowMouseCursor(true);
}

void ADDCharacterPlayer::Die()
{

}

void ADDCharacterPlayer::SetupCharacterWidget(UDDUserWidget* InUserWidget)
{
	UDDPlayerHPBarWidget* HpBarWidget = Cast<UDDPlayerHPBarWidget>(InUserWidget);
	ensure(HpBarWidget);
	if (HpBarWidget)
	{
		// TODO : YSY or LJW Remove MagicNumber. Need to StatComponent
		Stat->SetHp(500.0f);
		HpBarWidget->UpdateStat(500.0f);
		HpBarWidget->UpdateHpBar(500.0f);
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UDDPlayerHPBarWidget::UpdateHpBar);
	}
}

float ADDCharacterPlayer::ApplyDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->ApplyDamage(DamageAmount);
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



