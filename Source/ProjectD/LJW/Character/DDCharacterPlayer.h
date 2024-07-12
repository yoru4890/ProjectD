// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Character/DDCharacterBase.h"
#include "InputActionValue.h"
#include "DDCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDCharacterPlayer : public ADDCharacterBase
{
	GENERATED_BODY()
	
public:
	ADDCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
//Character Control Section
protected:
	void SetCharacterControl();
	virtual void SetCharacterControlData(const class UDDCharacterControlData* CharacterControlData) override;

	UPROPERTY(EditAnywhere, Category = Character)
	float MouseSpeed;

//Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;


	

//Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

};
