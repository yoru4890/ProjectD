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
#pragma endregion

}


void ADDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) 
	{
		EnableInput(PlayerController);
	}
	
	SetCharacterControl();
	
	
}

void ADDCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Montage Delegate Bind
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ADDCharacterPlayer::OnUnequipMontageEnded);

	//AnimInstance Delegate Bind
	UDDPlayerAnimInstance* PlayerAnimInstance = Cast<UDDPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	WeaponSystem->OnSetAimingDelegate.BindUObject(PlayerAnimInstance, &UDDPlayerAnimInstance::SetIsAiming);
	WeaponSystem->OnGetAimingDelegate.BindUObject(PlayerAnimInstance, &UDDPlayerAnimInstance::GetIsAiming);
	WeaponSystem->OnSetWeaponIndexDelegate.BindUObject(PlayerAnimInstance, &UDDPlayerAnimInstance::SetWeaponIndex);


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

	
}

void ADDCharacterPlayer::SetCharacterControl()
{
	
	UDDCharacterControlData* NewPlayerControlData = CharacterControlManager;
	check(NewPlayerControlData);

	SetCharacterControlData(NewPlayerControlData);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
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
	CameraBoom->TargetOffset = CharacterControlData->TargetOffset;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	MouseSpeed = CharacterControlData->MouseSpeed;

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

void ADDCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X * MouseSpeed);
	AddControllerPitchInput(LookAxisVector.Y * MouseSpeed);

}

void ADDCharacterPlayer::Sprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 427.f;
	// Stat->BaseStat.RUnspeed;
}

void ADDCharacterPlayer::Walk(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 147.f;
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

void ADDCharacterPlayer::EquipMelee()
{
	if (!(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()))
	{
		WeaponSystem->EquipMeleeWeapon();

	}
}

void ADDCharacterPlayer::EquipRange()
{
	if (!(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()))
	{
		WeaponSystem->EquipRangeWeapon();
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

