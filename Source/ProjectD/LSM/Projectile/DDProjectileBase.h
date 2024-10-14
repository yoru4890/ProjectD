// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDProjectileData.h"
#include "DDProjectileBase.generated.h"

UCLASS()
class PROJECTD_API ADDProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDProjectileBase();
	virtual ~ADDProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

public:

public:
	FORCEINLINE const FName& GetRowName() const { return RowName; }
	FORCEINLINE void SetRowName(const FName& InRowName) { RowName = InRowName; }

public:
	virtual void SetAssetAndManager(const FDDProjectileData& LoadedAsset, class  UDDProjectileManager* InProjectileManager);
	void ConfigureProjectile(float InDamageAmount, TSubclassOf<UDamageType> InDamageType, float InProjectileSpeed, float InMaxSpeed, float InMaxLifeTime, bool InbIsExplosive, float InExplosionRadius, int32 InMaxPenetrationCount);
	void SetProjectileState(bool bIsActive);

protected:
	UFUNCTION()
	virtual void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	virtual void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void LaunchProjectile();

	void ReturnToPool();

	void ApplyDamageToActor(AActor* OtherActor);

private:
	void SetupCollisionResponses();
	void SetParticeEffects(const FDDProjectileData& LoadedAsset);

	//UFUNCTION()
	//void OnTrailEffectFinished(class UNiagaraComponent* PSystem);

	//UFUNCTION()
	//void OnImpactEffectFinished(class UNiagaraComponent* PSystem);

	//UFUNCTION()
	//void OnExplosionEffectFinished(class UNiagaraComponent* PSystem);

	void SetSound(const FDDProjectileData& LoadedAsset);
	void SetAttachAudioComponent();
	void SetMeshs(const FDDProjectileData& LoadedAsset);

	void OnLifeTimeExpired();
	void StopLifeTimeTimer();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> FlyingAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> ImpactEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> TrailEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> ExplosionEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> FlyingSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosive", meta = (AllowPrivateAccess = "true"))
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Penetration", meta = (AllowPrivateAccess = "true"))
	int32 MaxPenetrationCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Penetration", meta = (AllowPrivateAccess = "true"))
	int32 CurrentPenetrationCount;

private:
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ProjectileAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lifetime", meta = (AllowPrivateAccess = "true"))
	float MaxLifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosive", meta = (AllowPrivateAccess = "true"))
	bool bIsExplosive;

	UPROPERTY()
	TObjectPtr<class UDDProjectileManager> ProjectileManager;

	FTimerHandle LifeSpanTimerHandle;

	bool bIsInPool;

	float HeightToPool = -2000.f;

	bool bTrailEffectFinished;
	bool bImpactEffectFinished;
	bool bExplosionEffectFinished;

};
