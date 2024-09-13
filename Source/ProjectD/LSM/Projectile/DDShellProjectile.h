// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Projectile/DDProjectileBase.h"
#include "DDShellProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDShellProjectile : public ADDProjectileBase
{
	GENERATED_BODY()

protected:
	virtual void SetAssetAndManager(const FDDProjectileData& LoadedAsset, class  UDDProjectileManager* InProjectileManager) override;
	void SetAttachNiagaraComponent();

	virtual void LaunchProjectile() override;

	virtual void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UFUNCTION()
	void OnTrailEffectFinished(class UNiagaraComponent* PSystem);

	UFUNCTION()
	void OnImpactEffectFinished(class UNiagaraComponent* PSystem);

	void HandleEffectCompletion();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> TrailNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> ImpactNiagaraComponent;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> DamagedActors;

	bool bTrailEffectFinished;
	bool bImpactEffectFinished;
};
