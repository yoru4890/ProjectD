// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Character/DDCharacterBase.h"
#include "InputActionValue.h"
#include "LJW/Interface/CameraFOVInterface.h"
#include "LJW/Interface/DDPlayerComponentsAnimInterface.h"
#include "YSY/Interface/AggroTargetInterface.h"
#include "YSY/Interface/DDCharacterWidgetInterface.h"
#include "YSY/Interface/DamageInterface.h"
#include "DDCharacterPlayer.generated.h"


UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	CombatMode UMETA(DisplayName = "CombatMode"),
	BuildMode UMETA(DisplayName = "BuildMode"),
	ManagementMode UMETA(DisplayName = "ManagementMode"),
	Unknow UMETA(DisplayName = "Unknown")
};

UCLASS()
class PROJECTD_API ADDCharacterPlayer : public ADDCharacterBase, public ICameraFOVInterface, public IDDPlayerComponentsAnimInterface, public IAggroTargetInterface, public IDDCharacterWidgetInterface, public IDamageInterface
{
	GENERATED_BODY()
	
public:
	ADDCharacterPlayer();

protected:
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	EPlayerMode CurrentPlayerMode;

//Character Control Section
protected:
	void SetCharacterControl();

	virtual void SetCharacterControlData(const class UDDCharacterControlData* CharacterControlData) override;

	UPROPERTY(VisibleAnywhere, Category = Control)
	TObjectPtr<APlayerController> PlayerController;

//Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;


//Input Section
protected:
	void Move(const FInputActionValue& Value);
	void BackMoveTrue();
	void BackMoveFalse();
	void Look(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Walk(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BackMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipMeleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipRangeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SubSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EnterManagementModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EnterBuildModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WaveStartAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PlaceBuildingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CancleBuildModeAction;

	UPROPERTY(EditAnywhere, Category = Character)
	float MouseSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsBackMove;

//Mesh Section
protected:

	void CreateLeaderPoseSkeletalMesh(USkeletalMeshComponent* USkeletalMesh, const FString& Name, const FString& Path );


	UPROPERTY(VisibleAnywhere, Category = Anim)
	TObjectPtr<class UDDPlayerAnimInstance> PlayerAnimInstance;

//Weapon System

public:
	UFUNCTION()
	void OnUnequipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ICameraFOVInterface
	virtual void SetCameraFOV(const float& Amount) override;

	virtual class UDDWeaponSystemComponent* GetWeaponComp() override;

protected:
	void EquipMelee();
	void EquipRange();
	void WeaponSubSkill();
	void WeaponStartAiming();
	void WeaponEndAiming();
	void WeaponAttack();
	void WeaponReload();

protected:

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<class UDDWeaponSystemComponent> WeaponSystem;

//AggroTarget System
public:
	virtual void AddAggro();
	virtual void SubtractAggro();
	virtual bool IsMaxAggro();

protected:
	int32 CurrentAggroNum{};
	int32 MaxAggroNum{ 3 };

//BuildComponent
public:
	UFUNCTION(BlueprintCallable)
	void EnterManagementMode();

	UFUNCTION(BlueprintCallable)
	void OpenBuildWidget();

	UFUNCTION(BlueprintCallable)
	void BuildTrapOrTower();

	UFUNCTION(BlueprintCallable)
	void PlaceBuilding();

	UFUNCTION(BlueprintCallable)
	void CancleBuildMode();

	void BindBuildingEvents();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Build, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDDBuildComponent> BuildSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Build, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> BuildHudNiagaraComponent;

// Wave System
public:
	void WaveStart();

//Input Mode
public:
	void SetPlayerCanNotMoveMode();
	void SetPlayerMoveOnlyMode();
	void SetPlayerGameMode();


protected:

// Stat
public:
	void Die();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Build, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDDCharacterStatComponent> Stat;

// CharacterWidgetInterface
public:
	virtual void SetupCharacterWidget(class UDDUserWidget* InUserWidget);

// DamageInterface
public:
	virtual float ApplyDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual void ApplyDamageOverTime(EDotDamageType DamageType, float Time, float TimeInterval, float DamageAmount);
	virtual void ApplyChainDamage(int DamageAmount, int NumberOfChain);
	virtual void ApplyDebuff(EDebuffType DebuffType, float Time, float DebuffRate);
};
