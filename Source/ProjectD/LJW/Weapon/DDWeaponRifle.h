// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJW/Weapon/DDWeaponBase.h"
#include "DDWeaponRifle.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadedAmmoChanged, int32, CurrentLoadedAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnLoadedAmmoChanged, int32, CurrentUnLoadedAmmo);

UCLASS()
class PROJECTD_API ADDWeaponRifle : public ADDWeaponBase
{
	GENERATED_BODY()
	
	ADDWeaponRifle();
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLoadedAmmoChanged OnLoadedAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUnLoadedAmmoChanged OnUnLoadedAmmoChanged;

	FORCEINLINE UAnimMontage* GetReloadMontage() { return ReloadAnim; }
	virtual void SubSkill() override;
	virtual void Attack() override;
	virtual void ResetWeaponSound() override;
	virtual void ResetWeaponState() override;
	bool Reload();
	void FireEmptyGun();

	// Ammo
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	FORCEINLINE int32 GetLoadedAmmo() const { return LoadedAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	FORCEINLINE int32 GetUnloadedAmmo() const { return UnLoadedAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool AddLoadedRifleAmmo();

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool SubtractLoadedRifleAmmo(int32 InAmmo);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void AddUnloadedRifleAmmo(const int32 InAmmo);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool SubtractUnloadedRifleAmmo(const int32 InAmmo);

private:

	void EffectAttack();


	UPROPERTY()
	TObjectPtr<class ADDPlayerState> PlayerState;

	UPROPERTY()
	TObjectPtr<class APlayerCameraManager> CameraManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> EmptyGunSound;
	
	const float AttackRange = 2500.0f;

	const int32 MaxAmmoPerMagazine = 30;

	const int32 MaxUnLoadedAmmo = 120;

	const int32 InitialLoadedAmmo = 30;
	const int32 InitialUnLoadedAmmo = 60;
	int32 LoadedAmmo;
	int32 UnLoadedAmmo;

	bool IsPlayEmptySound;
};
