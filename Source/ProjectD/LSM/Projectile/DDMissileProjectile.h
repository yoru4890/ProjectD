// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSM/Projectile/DDProjectileBase.h"
#include "DDMissileProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API ADDMissileProjectile : public ADDProjectileBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	void SetTargetActor(AActor* InTargetActor);

protected:
	virtual void LaunchProjectile() override;
	virtual void SetAssetAndManager(const FDDProjectileData& LoadedAsset, class  UDDProjectileManager* InProjectileManager) override;
	virtual void SetAttachNiagaraComponent() override;

	void ReachApexAndStartDescent();

	void ApplyAscentAcceleration(float DeltaTime);
	void ApplyDescentAcceleration(float DeltaTime);

	virtual void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void ApplyRadialDamage();
	
private:

	UPROPERTY(VisibleAnyWhere)
	TObjectPtr<AActor> TargetActor;
	FVector TargetLocation;
	FRotator TargetRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> TrailNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> ExplosionNiagaraComponent;

	UFUNCTION()
	void OnTrailEffectFinished(class UNiagaraComponent* PSystem);

	UFUNCTION()
	void OnExplosionEffectFinished(class UNiagaraComponent* PSystem);

	void HandleEffectCompletion();

	bool bApexHeightForRotate = false;
	bool bIsDescending = false;
	bool bIsMissileHidden = false;

	bool bTrailEffectFinished;
	bool bExplosionEffectFinished;


	float DescentAcceleration;
	float AscentAcceleration;
	float ApexHeightForVisible = 15000.f;  // 안보일 고도, 다시 보일 고도
	float ApexHeightForRotate = 20000.f;  // 적을 향해 회전할 고도
};
