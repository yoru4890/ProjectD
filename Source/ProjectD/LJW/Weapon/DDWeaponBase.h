// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LJW/Weapon/DDWeaponData.h"
#include "DDWeaponBase.generated.h"



UCLASS()
class PROJECTD_API ADDWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDWeaponBase();

	void InitData(const FName& RowName, const FDDWeaponData& WeaponData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	USkeletalMeshComponent* WeaponSkeletal;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName WeaponRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString WeaponDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FString WeaponInformation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 Price;

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
