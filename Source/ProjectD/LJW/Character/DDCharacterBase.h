// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DDCharacterBase.generated.h"


UCLASS()
class PROJECTD_API ADDCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADDCharacterBase();

	//Initialize Character state
	virtual void PostInitializeComponents() override;

protected:
	
	//Initialize Character Control
	virtual void SetCharacterControlData(const class UDDCharacterControlData* CharacterControlData);
	
	//Character Control Data
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDDCharacterControlData> CharacterControlManager;



};
