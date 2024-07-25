// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDWeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee,
	Range,
	Unknown
};

UCLASS()
class PROJECTD_API ADDWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FString WeaponInformation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 BuildCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 UnlockLP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float AttackCooltime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float AttackRange;
};
