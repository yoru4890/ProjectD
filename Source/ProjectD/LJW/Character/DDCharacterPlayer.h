// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Character/DDCharacterBase.h"
#include "InputActionValue.h"
#include "LJW/Interface/CameraFOVInterface.h"
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
class PROJECTD_API ADDCharacterPlayer : public ADDCharacterBase, public ICameraFOVInterface
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	EPlayerMode CurrentPlayerMode;

//Character Control Section
protected:
	void SetCharacterControl();

	virtual void SetCharacterControlData(const class UDDCharacterControlData* CharacterControlData) override;

//Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;


//Input Section
protected:
	void Move(const FInputActionValue& Value);
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
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipMeleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipRangeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SubSkillAction;

	UPROPERTY(EditAnywhere, Category = Character)
	float MouseSpeed;


//Mesh Section
protected:

	void CreateLeaderPoseSkeletalMesh(USkeletalMeshComponent* USkeletalMesh, const FString& Name, const FString& Path );


//Weapon System

public:
	UFUNCTION()
	void OnUnequipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ICameraFOVInterface
	virtual void SetCameraFOV(const float& Amount) override;

protected:
	void EquipMelee();
	void EquipRange();
	void WeaponSubSkill();
	void WeaponStartAiming();
	void WeaponEndAiming();


protected:

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<class UDDWeaponSystemComponent> WeaponSystem;


};
